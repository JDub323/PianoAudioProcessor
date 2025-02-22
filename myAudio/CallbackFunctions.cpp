//
// Created by jwhal on 11/20/2024.
//

#include "CallbackFunctions.h"

#include <cstdio>
#include <fftw3.h>
#include <iostream>
#include <cmath>

#include "AudioFinder.h"
#include "../noteLogic/SpectroHandler.h"


float fastExp2(const float x) {
    const int n = static_cast<int>(x);
    const float fractional = x - n;
    // Approximate 2^fractional using linear interpolation: 2^fractional ≈ 1 + fractional
    return static_cast<float>(1 << n) * (1.0f + fractional);
}

int CallbackFunctions::frequencyDomainAmplitudeDisplay(const void *inputBuffer, void *outputBuffer,
                                                      unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {

    auto* in = (float*)inputBuffer;
        (void)outputBuffer;
        auto* callbackData = static_cast<streamCallbackData *>(userData);

        constexpr int displaySize = 50;
        printf("\r");

        for (unsigned long i = 0; i < framesPerBuffer; i++) {
            callbackData->in[i] = in[i * NUM_CHANNELS];
        }

        fftw_execute(callbackData->p);

        //prints fft data to the terminal
        for (int i = 0; i < displaySize; i++) {
            const double frequencyIndex = callbackData->startIndex * fastExp2(i * progressConstant / static_cast<double>(displaySize));
            const double *complex = callbackData->out[static_cast<int>(frequencyIndex)];
            const double frequencyAmplitude = hypot(complex[0], complex[1]) * SCALING_FACTOR / FRAMES_PER_BUFFER;

            if (frequencyAmplitude < .125) {//`▁`, `▂`, `▃`, `▄`, `▅`, `▆`, `▇`, `█`
                std::cout<<".";
            }
            else if (frequencyAmplitude< .25) {
                std::cout<<"▁";
            }
            else if (frequencyAmplitude< .375) {
                std::cout<<"▂";
            }
            else if (frequencyAmplitude< .5) {
                std::cout<<"▃";
            }
            else if (frequencyAmplitude< .625) {
                std::cout<<"▄";
            }
            else if (frequencyAmplitude< .75) {
                std::cout<<"▅";
            }
            else if (frequencyAmplitude< .875) {
                std::cout<<"▆";
            }
            else {
                std::cout<<"▇";
            }
        }

        fflush(stdout);

        return 0;
}


int CallbackFunctions::basicPianoDomainAmplitudeDisplay(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    auto* in = (float*)inputBuffer;
    (void)outputBuffer;
    auto* callbackData = static_cast<streamCallbackData *>(userData);

    constexpr int displaySize = 88;
    printf("\r");

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        callbackData->in[i] = in[i * NUM_CHANNELS];
    }

    fftw_execute(callbackData->p);

    double currentFrequency = 27.5;
    const double twelfthRootTwo = std::pow(2.0,1.0/12.0);
    constexpr double frequencyResolution = SAMPLE_RATE/FRAMES_PER_BUFFER;
    for (int i = 0; i < displaySize; i++) {
        currentFrequency *= twelfthRootTwo;
        const int frequencyIndex = static_cast<int>(currentFrequency / frequencyResolution);
        const double *complex = callbackData->out[frequencyIndex];
        const double frequencyAmplitude = hypot(complex[0], complex[1]) * SCALING_FACTOR / FRAMES_PER_BUFFER;

        if (frequencyAmplitude < .125) {//`▁`, `▂`, `▃`, `▄`, `▅`, `▆`, `▇`, `█`
            std::cout<<".";
        }
        else if (frequencyAmplitude< .25) {
            std::cout<<"▁";
        }
        else if (frequencyAmplitude< .375) {
            std::cout<<"▂";
        }
        else if (frequencyAmplitude< .5) {
            std::cout<<"▃";
        }
        else if (frequencyAmplitude< .625) {
            std::cout<<"▄";
        }
        else if (frequencyAmplitude< .75) {
            std::cout<<"▅";
        }
        else if (frequencyAmplitude< .875) {
            std::cout<<"▆";
        }
        else {
            std::cout<<"▇";
        }
    }

    fflush(stdout);

    return 0;
}


int CallbackFunctions::noDisplay(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    auto* in = (float*)inputBuffer;
    (void)outputBuffer;
    auto* callbackData = static_cast<streamCallbackData *>(userData);

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        callbackData->in[i] = in[i * NUM_CHANNELS];
    }

    fftw_execute(callbackData->p);

    return 0;
}

int CallbackFunctions::noDisplaySaveMagnitudeHistory(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    auto* in = (float*)inputBuffer;
    (void)outputBuffer;
    auto* callbackData = static_cast<streamCallbackData *>(userData);

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        callbackData->in[i] = in[i * NUM_CHANNELS];
    }

    fftw_execute(callbackData->p);

    SpectroHandler::saveSpectroData();

    return 0;
}

