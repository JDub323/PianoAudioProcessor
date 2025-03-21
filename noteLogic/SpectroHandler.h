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

//This struct is meant for audio input, taking the STFT of audio data from the mic or speaker
typedef struct {
    double* in;
    fftw_complex* out;
    fftw_plan p;
    int startIndex;
    int spectrogramSize;
} streamCallbackData;

//This struct is meant for audio output, designed to take the reverse fourier transform from some file
typedef struct {
    fftw_complex* in;//data taken from file
    double* out;//.wav data which is put into the portaudio output buffer
    fftw_plan p;//fftw plan for the reverse fourier transform half complex to real
    int startIndex;//start Index of spectrogram
    int spectrogramSize;//size of spectrogram data from file
} streamPlaybackData;

class SpectroHandler {
private:
    //acts as a circular buffer mediating the data from the file and the program in a portaudio function
    //to use, call the getNextSpectrogram() function to, as you may guess, get the next spectrogram.
    //currentFileIndex is augmented automatically. To load the next buffer, call loadNextBuffer().
    //to know if the next buffer should be loaded, call the bool needToLoadNextBuffer().
    //should start a thread to
    static fftw_complex* spectrogramFileData;
    static int currentFileIndex;

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

    //functions to manipulate the

    //functions for audio input
    static void initializeSpectrogramData();
    static void allocateMagnitudeHistoryMemory(bool saveKeysOnly);
    static void deallocateSpectrogramData();
    static void deallocateMagnitudeHistoryMemory();

    //functions for audio output
    static void initializePlaybackSpectroData();
    static void initializeFileData();//include checks to make sure the saved format is the same as the current format
    static void deallocatePlaybackSpectroData();
    static void deallocateFileData();

    static void saveSpectroData();

    static double getMagnitudeAt(int index);
    static uint8_t shortenDoubleToByte(double in); //I am considering making this into a short, doubling the precision to a 16 bit standard

    static double convertToFrequency(int index);
    static int convertToSpectroIndex(float frequency);
};



#endif //SPECTROHANDLER_H
