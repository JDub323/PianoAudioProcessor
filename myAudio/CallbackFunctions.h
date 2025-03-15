//
// Created by jwhal on 11/20/2024.
//

#ifndef CALLBACKFUNCTIONS_H
#define CALLBACKFUNCTIONS_H
#include <cmath>
#include <fftw3.h>
#include <portaudio.h>
#include "AudioFinder.h"



//evaluated from the equation START_FREQ * 2^(progress * K) = END_FREQ, when this is multiplied by percent progress,
//will allow a curve of START_FREQ * 2^(i*k) to start at starting frequency, end at ending frequency, and have a nice,
//exponential curve. To be similar to a piano, I use log base 2, although any log could technically be used.
static const double progressConstant = std::log2(END_FREQUENCY/START_FREQUENCY);

typedef int (PaCallbackFunction)(const void* inputBuffer, void* outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void* userData);

class CallbackFunctions {
public:
    //INPUT CALLBACK FUNCTIONS
    static PaCallbackFunction frequencyDomainAmplitudeDisplay;
    static PaCallbackFunction basicPianoDomainAmplitudeDisplay;
    static PaCallbackFunction displayTopFiveDetectedNotes;
    static PaCallbackFunction noDisplay;
    static PaCallbackFunction noDisplaySaveMagnitudeHistory;

    //OUTPUT CALLBACK FUNCTIONS
    static PaCallbackFunction noDisplayPlaySineWave;
    static PaCallbackFunction noDisplayPlayQueuedAudio;//TODO
};



#endif //CALLBACKFUNCTIONS_H
