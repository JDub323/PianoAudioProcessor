//
// Created by jwhal on 11/19/2024.
//

#include "AudioFinder.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "CallbackFunctions.h"


static PaStream* stream;

static void checkErr(const PaError err) {
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        Pa_Terminate();
        exit(EXIT_FAILURE);
    }
}

void suppressAllErrors(const bool suppress) {
    if (suppress) {
        freopen("NUL", "w", stderr);
    }
}

//initialize portaudio and check for errors. Will mute all errors to mute portaudio logging messages if the
//macro SHOW_PA_ERRORS is false, so be careful that other runtime errors aren't also suppressed.
void AudioFinder::initializePortAudio() {
    //stops portaudio logging messages from printing to the terminal, also prevents actual runtime errors from being printed,
    //so be careful with this
    suppressAllErrors(!SHOW_PA_ERRORS);

    PaError err;
    err = Pa_Initialize();
    checkErr(err);
}

//initializePortAudio() must have been called previously. Will initialize the input audio stream based on the variable
//"currentDevice", and will set the output stream to nullptr. This will call Pa_OpenStream() based on the current
//device, NUM_CHANNELS, SAMPLE RATE, FRAMES_PER_BUFFER, and whatever callbackFunction and userdata pointers are
//available, and will then start the stream with Pa_StartStream()
void AudioFinder::startRecording(PaCallbackFunction* callbackFunction, void *userData) {
    constexpr int inputDevice = currentDevice.deviceNum;

    PaStreamParameters inputParameters = {};

    //initialize input stream parameters for portaudio
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.device = inputDevice;
    inputParameters.hostApiSpecificStreamInfo = nullptr;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputDevice)->defaultLowInputLatency;

    PaError err = Pa_OpenStream(
        &stream,
        &inputParameters,
        nullptr,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paNoFlag,
        *callbackFunction,
        userData
    );
    checkErr(err);

    err = Pa_StartStream(stream);
    checkErr(err);

}

//Stops the PaStream & checks for errors. Should be called before quitAndDeallocate()
void AudioFinder::pausePortAudio() {
    const PaError err = Pa_StopStream(stream);
    checkErr(err);
}

//Closes the PaStream and deallocates the portaudio memory. Should be called after pausePortAudio, probably.
//Does not deallocate spectrogramData, this is done by calling SpectroHandler::deallocateSpectrogramData()
void AudioFinder::quitAndDeallocate() {
    PaError err = Pa_CloseStream(stream);
    checkErr(err);

    err = Pa_Terminate();
    checkErr(err);
}

AudioSettings AudioFinder::getAudioSettings() {
    return currentSettings;
}

//initializePortAudio() must have been called previously. Will use the microphone defined by "currentDevice" variable.
//
void AudioFinder::startPlaying(PaCallbackFunction callbackFunction, void *userData) {
    constexpr int outputDevice = currentDevice.deviceNum;

    PaStreamParameters outputParameters = {};

    //initialize input stream parameters for portaudio
    outputParameters.channelCount = NUM_CHANNELS;
    outputParameters.device = outputDevice;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputDevice)->defaultLowOutputLatency;

    PaError err = Pa_OpenStream(
            &stream,
            nullptr,
            &outputParameters,
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paNoFlag,
            *callbackFunction,
            userData
    );
    checkErr(err);

    err = Pa_StartStream(stream);
    checkErr(err);
}



