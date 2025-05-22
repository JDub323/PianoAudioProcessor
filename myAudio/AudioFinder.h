//
// Created by jwhal on 11/19/2024.
//

#ifndef AUDIOFINDER_H
#define AUDIOFINDER_H

#define SHOW_PA_ERRORS false
#define SHOW_MIC_OPTIONS false
#define SHOW_NUM_DEVICES false
#define SAMPLE_RATE currentDevice.sampleRate
#define FRAMES_PER_BUFFER 4096
#define SCALING_FACTOR 40    //keep this low when analyzing audio to avoid aliasing, but this can stay high when looking at images, as a rule, 10-30+ for images, and 1-3 for saving them
#define NUM_CHANNELS currentDevice.numChannels
#define START_FREQUENCY 20
#define END_FREQUENCY 20000
#define RECORDING_TIME 10
#define MAX_INTERESTING_FREQUENCY 10000

#include <portaudio.h>
#include <cstdio>

#include "../noteLogic/SpectroHandler.h"

//struct contains which device which is being played, the sampling rate, and the number of channels. This is used
//to quickly switch between devices. If not using my computer, see below comments
typedef struct {
    int deviceNum;
    double sampleRate;
    int numChannels;
} deviceSettings;

//Below are some common input and output devices I may want to use for my application. This will vary depending
//on one's computer. To find your own settings, print out all compatible microphone options (which is a function in
//UnitTests.cpp) and use something with 44.1 kHz+ of sample rate and 1-2 channels of input or output (input if you
//want to use data from what is being played/recorded in your application, output if you want to play something on
//a speaker). Trial and error is necessary.

//INPUT DEVICES
static constexpr deviceSettings laptopMic = {1, 44100.0, 2};
static constexpr deviceSettings laptopInSpeaker = {19, 48000.0, 2};
static constexpr deviceSettings airpodsProInSpeaker = {30, 48000.0, 2};
static constexpr deviceSettings airpodsProMic = {0,44100.0,1};
static constexpr deviceSettings newAirpodsInSpeaker = {28,48000.0,2};
static constexpr deviceSettings newAirpodsMic = {3,44100.0,1};
static constexpr deviceSettings laptopMicWithSecondMonitor = {1, 44100.0, 2};
static constexpr deviceSettings laptopInSpeakerWithSecondMonitor = {27, 48000.0, 2};

//OUTPUT DEVICES
//(much empty)

//CURRENT DEVICE
static constexpr deviceSettings currentDevice = laptopMic;


//these are all the parameters needed for a portaudio callback function. These functions are called every
//SAMPLES_PER_SECOND/FRAMES_PER_BUFFER, and need to
typedef int (PaCallbackFunction)(const void* inputBuffer, void* outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void* userData);


//these are the audio settings saved in files when saving files. Saving this info lets one ensure invalid files are not
//played (ie: if the samples per buffer does not match what was saved and there would be large distortions)
typedef struct {
    const double samplesPerSecond;
    const int samplesPerBuffer;
    const int spectroWidth;
    const int spectroFirstIndex;
} AudioSettings;

//the current settings my program is using. Used to check if the current settings match what settings were saved in the
//file saved last time
static AudioSettings currentSettings = {SAMPLE_RATE, FRAMES_PER_BUFFER,
    SpectroHandler::SPECTROGRAM_SIZE, static_cast<int>(std::ceil(FRAMES_PER_BUFFER / SAMPLE_RATE * START_FREQUENCY))};

class AudioFinder {
public:
    static void initializePortAudio();
    static void startRecording(PaCallbackFunction* callbackFunction, void *userData);
    static void startPlaying(PaCallbackFunction callbackFunction, void *userData);
    static void pausePortAudio();
    static void quitAndDeallocate();
    static AudioSettings getAudioSettings();



    //TODO: gather same note twice and measure the error with that one function I wrote two weeks ago
};



#endif //AUDIOFINDER_H