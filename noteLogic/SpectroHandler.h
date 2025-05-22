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

//this is the number of rows from the file that I have saved at a time.
#define FILE_BUFFER_SIZE 4

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
    //this is a 2d array of pointers, with size defined by bufferSize inside the function initializeFileData.
    //to use, call the getNextSpectrogram() function to, as you may guess, get the next spectrogram.
    //currentFileIndex is augmented automatically.
    //a thread should be run that only sleeps and checks to see if more from the file should be loaded, in
    //which case, it should load it, obviously.
    static fftw_complex** spectrogramFileData;
    //keeps track of location in file
    static int currentFileIndex;
    static void beginBufferThread();//TODO


public:
    //contains data on input and output buffers, the plan used for the callback function, the beginning index of the
    //spectrogram, and the size of the spectrogram
    static streamCallbackData* spectrogramData;

    //the equivalent of spectrogramData, but for audio output
    static streamPlaybackData* playbackData;

    //the size of the spectrogram, as calculated by the macro
    const static int SPECTROGRAM_SIZE;
    //the total number of samples, as calculated by the macro
    const static int TOTAL_SAMPLES;
    //the total number of samples taken (the height of a spectrogram image)
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

    //functions for saving audio input
    //initializeSpectrogramData(); (use the one above)
    static void allocateFFTHistoryMemory();//TODO fxn
    //deallocateSpectrogramData();
    static void deallocateFFTHistoryMemory();//TODO fxn

    //functions for audio output
    static void initializeStreamPlaybackData();
    static void initializeFileData(char* fileName);//include checks to make sure the saved format is the same as the current format
    static void deallocateStreamPlaybackData();
    static void deallocateFileData();

    //functions to save Spectro Data to Magnitude History
    static void saveSpectroDataToMagnitudeHistory();
    static void saveSpectroDataToMagnitudeHistory(const streamCallbackData * spectrogramData);
    static void getNextSpectrogram();//TODO (IDK what I wanted this one to do :/ )

    //functions to save spectro data to fftHistory
    static void saveSpectroDataToFFTHistory();


    static double getMagnitudeAt(int index);
    static double getMagnitudeAt(streamCallbackData * spectrogramData, int index);
    static uint8_t shortenDoubleToByte(double in); //I am considering making this into a short, doubling the precision to a 16 bit standard

    static double convertToFrequency(int index);
    static int convertToSpectroIndex(float frequency);
};



#endif //SPECTROHANDLER_H
