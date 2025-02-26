//
// Created by jwhal on 11/19/2024.
//

#ifndef AUDIOFINDER_H
#define AUDIOFINDER_H

#define SHOW_PA_ERRORS false
#define SHOW_MIC_OPTIONS false
#define SHOW_NUM_DEVICES false
#define SAMPLE_RATE currentDevice.sampleRate
#define FRAMES_PER_BUFFER 8192
#define SCALING_FACTOR 30    //keep this low when analyzing audio to avoid aliasing, but this can stay high when looking at images, as a rule, 10-30+ for images, and 1-3 for saving them
#define NUM_CHANNELS currentDevice.numChannels
#define START_FREQUENCY 20
#define END_FREQUENCY 20000
#define RECORDING_TIME 5
#define MAX_INTERESTING_FREQUENCY 10000

#include <portaudio.h>

typedef struct {
    int deviceNum;
    double sampleRate;
    int numChannels;
} deviceSettings;

typedef int (PaCallbackFunction)(const void* inputBuffer, void* outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void* userData);

static constexpr deviceSettings laptopMic = {1, 44100.0, 2};
static constexpr deviceSettings laptopSpeaker = {24, 48000.0, 2};
static constexpr deviceSettings airpodsProSpeaker = {30, 48000.0, 2};
static constexpr deviceSettings airpodsProMic = {0,44100.0,1};
static constexpr deviceSettings newAirpodsSpeaker = {28,48000.0,2};
static constexpr deviceSettings newAirpodsMic = {3,44100.0,1};
static constexpr deviceSettings laptopMicWithSecondMonitor = {1, 44100.0, 2};
static constexpr deviceSettings laptopSpeakerWithSecondMonitor = {27, 48000.0, 2};

static constexpr deviceSettings currentDevice = laptopSpeakerWithSecondMonitor;

class AudioFinder {
public:
    static void connectToMic();
    static void startRecording(PaCallbackFunction* callbackFunction, void *userData);
    static void pauseRecording();
    static void micQuitAndDeallocate();

    static void connectToSpeaker();//TODO, likely need image file as input to get inverse fft ready
    static void startPlaying();//TODO
    static void pausePlaying();//TODO
    static void speakerQuitAndDeallocate();//TODO
};



#endif //AUDIOFINDER_H