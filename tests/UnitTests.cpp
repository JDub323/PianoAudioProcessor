#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>

#include <cmath>
#include <fftw3.h>
#include <portaudio.h>
#include <string>
#include <conio.h>  // for _kbhit() and _getch()
#include "../myAudio/AudioFinder.h"
#include "../myAudio/CallbackFunctions.h"
#include "../noteLogic/SpectroHandler.h"
#include "../myGui/GuiHandler.h"
#include "../mygui/SpectroImageMaker.h"
#include "../noteLogic/PianoLogic.h"

//displays fft to terminal based on the parameter PaCallbackFunction
void setTimeTerminalDisplayTest(const PaCallbackFunction function) {
    //allows half-block characters to be written in terminal
    SetConsoleOutputCP(CP_UTF8);

    AudioFinder::initializePortAudio();

    //initialize global variable spectrogramData
    SpectroHandler::initializeSpectrogramData();

    Pa_Sleep(1000);//wait for system to be ready with terminal so there are no f flush failures
    AudioFinder::startRecording(function, SpectroHandler::spectrogramData);

    Pa_Sleep(RECORDING_TIME * 1000);

    AudioFinder::pausePortAudio();

    AudioFinder::quitAndDeallocate();

    SpectroHandler::deallocateSpectrogramData();
}

void variableTimeTerminalDisplayTest(const PaCallbackFunction function) {
    //allows half-block characters to be written in terminal
    SetConsoleOutputCP(CP_UTF8);

    AudioFinder::initializePortAudio();

    //initialize global variable spectrogramData
    SpectroHandler::initializeSpectrogramData();

    Pa_Sleep(1000);//wait for system to be ready with terminal so there are no f flush failures

    AudioFinder::startRecording(function, SpectroHandler::spectrogramData);

    //yes chatGPT wrote this loop, sue me
    while (true) {
        if (_kbhit()) {  // Check if a key was pressed
            if (_getch() == '\r') {  // Check if it was Enter
                break;
            }
        }
        Pa_Sleep(100); // Sleep to prevent high CPU usage
    }

    AudioFinder::pausePortAudio();

    AudioFinder::quitAndDeallocate();

    SpectroHandler::deallocateSpectrogramData();
}

// void imguiFrequencyDisplayTest(const bool useLogarithmicScaling, const bool saveSpectrogramData) {
//     AudioFinder::initializePortAudio();                               //connect to mic
//     SpectroHandler::initializeSpectrogramData();               //initialize fftw
//     if (saveSpectrogramData) {
//         SpectroHandler::allocateMagnitudeHistoryMemory();      //allocate memory to save magnitude history
//     }
//     //start listening
//     AudioFinder::startRecording(CallbackFunctions::noDisplay, SpectroHandler::spectrogramData);
//     GuiHandler::initializeImgui();                 //make imgui window
//     GuiHandler::plotAudio(useLogarithmicScaling);  //make imgui plot
//
//     //code sleeps here until the window is closed
//
//     //handle closing & ending the plot?
//     AudioFinder::pausePortAudio();                 //stop listening
//     AudioFinder::quitAndDeallocate();              //deallocate Portaudio
//     GuiHandler::shutDownImgui();                   //close window
//
//     if (saveSpectrogramData) {                     //saves audio data if that option was selected
//         //add file where magnitudeHistory is written to
//         //make sure to limit the audio history to the first ten seconds of audio so not too much memory is used
//         SpectroHandler::deallocateMagnitudeHistoryMemory();
//         //printf("Audio successfully written to filename");
//     }
//     SpectroHandler::deallocateSpectrogramData();   //deallocate spectrogramData
// }

/*
 * listens for a set period of time as defined by audioFinder, but does not display to the terminal,
 * instead summing each of the buckets around a piano key and saving it on a pgm file. The file has a size
 */
void spectroImageFileTest(const bool saveOnlyPianoKeys) {
    AudioFinder::initializePortAudio();

    //initialize global variable spectrogramData
    SpectroHandler::initializeSpectrogramData();

    SpectroHandler::allocateMagnitudeHistoryMemory(saveOnlyPianoKeys);//allocate memory to save magnitude history

    Pa_Sleep(1000);//wait for system to be ready with terminal so there are no f flush failures
    AudioFinder::startRecording(CallbackFunctions::noDisplaySaveMagnitudeHistory, SpectroHandler::spectrogramData);

    Pa_Sleep(RECORDING_TIME * 1000);

    AudioFinder::pausePortAudio();

    AudioFinder::quitAndDeallocate();

    SpectroHandler::deallocateSpectrogramData();

    SpectroImageMaker::makePGMImage(SpectroHandler::spectrogramMagnitudeHistory, SpectroHandler::SPECTROGRAM_SIZE,
                                    SpectroHandler::TOTAL_SAMPLES / SpectroHandler::SPECTROGRAM_SIZE, saveOnlyPianoKeys, "audioPlot.pgm");

    SpectroHandler::deallocateMagnitudeHistoryMemory();
}

//prints all bucket indexes and frequencies, highlighting the buckets which have
//the same frequencies as multiple keys on a piano
void bucketCrossoverTest() {
    double currentFrequency = 27.5;
    const double twelfthRootTwo = std::pow(2.0,1.0/12.0);
    constexpr double frequencyResolution = static_cast<float>(MAX_INTERESTING_FREQUENCY) / FRAMES_PER_BUFFER;

    auto red = "\033[31m";
    auto white = "\033[37m";

    int previousFrequencyIndex = -1;
    int numCollisions = 0;

    for (int i = 0; i < 88; i++) {
        const int frequencyIndex = static_cast<int>(currentFrequency / frequencyResolution);

        if (previousFrequencyIndex == frequencyIndex) {
            printf(red); //makes text red
            numCollisions++;
        }
        else printf(white); //makes text white when there is no collision

        const double realFrequency = std::trunc(frequencyIndex) * frequencyResolution;
        printf("Key: %d, Index: %d, Frequency: %f, Want: %f\n", i+1, frequencyIndex, realFrequency, currentFrequency);

        previousFrequencyIndex = static_cast<int>(currentFrequency/frequencyResolution);
        currentFrequency *= twelfthRootTwo;
    }

    printf("Number of Collisions: %d", numCollisions);
}

//prints all available microphone options
void printAllMicOptions() {
    AudioFinder::initializePortAudio();

    const int numDevices = Pa_GetDeviceCount();

    printf("Number of devices: %d\n", numDevices);
    if (numDevices < 0) {
        printf("Error getting device count.\n");
        exit(EXIT_FAILURE);
    } if (numDevices == 0) {
        printf("There are no available audio devices on this machine.\n");
        exit(EXIT_SUCCESS);
    }


    //prints out all available options to link to with portaudio
    for (int i = 0; i < numDevices; i++) {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
        printf("Device %d:\n", i);
        printf("  name: %s\n", deviceInfo->name);
        fflush(stdout); //some names are too long, make sure all names are printed
        printf("  maxInputChannels: %d\n", deviceInfo->maxInputChannels);
        printf("  maxOutputChannels: %d\n", deviceInfo->maxOutputChannels);
        printf("  defaultSampleRate: %f\n", deviceInfo->defaultSampleRate);
    }

    if (const PaError err = Pa_Terminate(); err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        Pa_Terminate();
        exit(EXIT_FAILURE);
    }
}

void testFileMaker() {
    constexpr int width = 1024;
    constexpr int height = 1024;
    constexpr int size = width * height;

    auto* fakeSpectrogram = static_cast<uint8_t *>(malloc(sizeof(uint8_t) * size));
    for (int i = 0; i < size; i++) {
        fakeSpectrogram[i] = i;
    }
    SpectroImageMaker::makePGMImage(fakeSpectrogram, width, height, false, "testFile.pgm");

    free(fakeSpectrogram);
}

//used to determine if there is a bug in my spectroMagnitudeHistory saver or if it is a memory issue and I need
//to make a circular buffer to write to file in real time instead of all at once at the end of the life of the
//program.
//it is likely I will use a circular buffer anyway to save memory on low performance devices
//this function is basically spectroImageFileTest() but without accessing portaudio
//it works!! :)
void testSpectroSaver(const bool saveOnlyPianoKeys) {
    //AudioFinder::initializePortAudio();

    //initialize global variable spectrogramData
    SpectroHandler::initializeSpectrogramData();

    SpectroHandler::allocateMagnitudeHistoryMemory(saveOnlyPianoKeys);//allocate memory to save magnitude history

    //Pa_Sleep(1000);//wait for system to be ready with terminal so there are no f flush failures
    //AudioFinder::startRecording(CallbackFunctions::noDisplaySaveMagnitudeHistory, SpectroHandler::spectrogramData);

    for (int i = 0; i < 100; i++) {
        SpectroHandler::saveSpectroData();
    }

    //Pa_Sleep(RECORDING_TIME * 1000);

    //AudioFinder::pausePortAudio();

    //AudioFinder::quitAndDeallocate();

    SpectroHandler::deallocateSpectrogramData();

    SpectroImageMaker::makePGMImage(SpectroHandler::spectrogramMagnitudeHistory,SpectroHandler::SPECTROGRAM_SIZE,
                                    SpectroHandler::TOTAL_SAMPLES / SpectroHandler::SPECTROGRAM_SIZE, saveOnlyPianoKeys, "audioPlot.pgm");

    SpectroHandler::deallocateMagnitudeHistoryMemory();
}

void printSpectroSizes() {
    printf("SpectrogramSize: %d\n", SpectroHandler::SPECTROGRAM_SIZE);
    printf("numSamples: %d\n", SpectroHandler::TOTAL_SAMPLES);
    printf("height: %d\n", SpectroHandler::TOTAL_SAMPLES / SpectroHandler::SPECTROGRAM_SIZE);
}

void testSpectroConversions() {//this works except the spectro size is not necessarily what is input into the fft is
    for (int i = std::ceil(FRAMES_PER_BUFFER * START_FREQUENCY / SAMPLE_RATE); i < SpectroHandler::SPECTROGRAM_SIZE; i++) {
        const double freq = SpectroHandler::convertToFrequency(i);
        const int identity = SpectroHandler::convertToSpectroIndex(static_cast<float>(freq));
        printf("Bucket %d: %3d | ", i, identity);
        printf("%lf\n", freq);

        if (i != identity) printf("***********\n");
    }
}

void setTimeSpeakerTest(PaCallbackFunction function) {
    AudioFinder::initializePortAudio();

    //initialize global variable spectrogramData
    SpectroHandler::initializePlaybackSpectroData();

    Pa_Sleep(1000);//wait for system to be ready with terminal so there are no f flush failures
    AudioFinder::startRecording(function, SpectroHandler::spectrogramData);

    Pa_Sleep(RECORDING_TIME * 1000);

    AudioFinder::pausePortAudio();

    AudioFinder::quitAndDeallocate();

    SpectroHandler::deallocateSpectrogramData();
}

int main() {
    printAllMicOptions();
    //variableTimeTerminalDisplayTest(CallbackFunctions::displayTopFiveDetectedNotes);
    setTimeTerminalDisplayTest(CallbackFunctions::frequencyDomainAmplitudeDisplay);
    //bucketCrossoverTest();
    //spectroImageFileTest(false);
    //testFileMaker();
    //printSpectroSizes();
    //testSpectroSaver(false);


    return EXIT_SUCCESS;
}
