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
void AudioFinder::connectToMic() {
    //stops portaudio logging messages from printing to the terminal, also prevents actual runtime errors from being printed,
    //so be careful with this
    suppressAllErrors(!SHOW_PA_ERRORS);

    PaError err;
    err = Pa_Initialize();
    checkErr(err);
}

//connectToMic() must have been called previously. Will initialize the input audio stream based on the variable
//"currentDevice", and will set the output stream to nullptr. This will call Pa_OpenStream() based on the current
//device, NUM_CHANNELS, SAMPLE RATE, FRAMES_PER_BUFFER, and whatever callbackFunction and userdata pointers are
//available, and will then start the stream with Pa_StartStream()
void AudioFinder::startRecording(PaCallbackFunction* callbackFunction, void *userData) {
    //input devices that do listen to my audio (to any extent): 6,7,8,
    //use device 7 when recording audio: "Microphone Array (Intel┬« Smart Sound Technology for Digital Microphones)"
    //device 23 is what is playing on my laptop speaker: "PC Speaker (Realtek HD Audio output with SST)"
    constexpr int inputDevice = currentDevice.deviceNum; //say device 7 instead of using the getDeviceIndex method because there are bugs with the long name

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
void AudioFinder::pauseRecording() {
    const PaError err = Pa_StopStream(stream);
    checkErr(err);
}

//Closes the PaStream and deallocates the portaudio memory. Should be called after pauseRecording, probably.
//Does not deallocate spectrogramData, this is done by calling SpectroHandler::deallocateSpectrogramData()
void AudioFinder::micQuitAndDeallocate() {
    PaError err = Pa_CloseStream(stream);
    checkErr(err);

    err = Pa_Terminate();
    checkErr(err);
}