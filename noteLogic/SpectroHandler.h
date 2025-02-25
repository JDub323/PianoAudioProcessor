//
// Created by jwhal on 11/20/2024.
//

#ifndef SPECTROHANDLER_H
#define SPECTROHANDLER_H

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fftw3.h>

#define AMPLIFY_HIGH_FREQUENCIES false
//here loudness is defined as the riemann sum of all of the
#define CONSTANT_LOUDNESS_OVER_TIME false

typedef struct {
    double* in;
    fftw_complex* out;
    fftw_plan p;
    int startIndex;
    int spectrogramSize;
} streamCallbackData;

class SpectroHandler {
public:
    //contains data on input and output buffers, the plan used for the callback function, the beginning index of the
    //spectrogram, and the size of the spectrogram
    static streamCallbackData* spectrogramData;

    const static int SPECTROGRAM_SIZE;
    const static int TOTAL_SAMPLES;
    const static int TIMES_SAMPLED;

    //contains data for every magnitude for every frequency bucket for every measurement in time. Uses a 1d array
    //to save time. Stores every bucket from fft from one buffer contiguously, before going to the next buffer.
    //to find a certain frequency from a certain buffer, use the formula: SAMPLING_RATE * k / FRAMES_PER_BUFFER
    //to find a certain frequency at a certain time, use the formula:
    //with a recording time of 10 seconds, can expect the array to be ~1/2 megabyte of data
    static uint8_t* spectrogramMagnitudeHistory;
    static int currentMagnitudeIndex;

    static void initializeSpectrogramData();
    static void allocateMagnitudeHistoryMemory(bool saveKeysOnly);
    static void deallocateSpectrogramData();
    static void deallocateMagnitudeHistoryMemory();

    static void saveSpectroData();

    static double getMagnitudeAt(int index);
    static uint8_t shortenDoubleToByte(double in); //I am considering making this into a short, doubling the precision to a 16 bit standard

    static double convertToFrequency(int index);
    static int convertToSpectroIndex(float frequency);
};



#endif //SPECTROHANDLER_H
