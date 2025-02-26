//
// Created by jwhal on 11/20/2024.
//

#include "SpectroHandler.h"
#include "../myAudio/AudioFinder.h"

#include <cmath>
#include <cstdlib>
#include <vector>

#include "../myGui/SpectroImageMaker.h"

//precalculated width of the spectrogram. Uses MAX_INTERESTING_FREQUENCY as the maximum frequency produced by a piano, so no memory is allocated which will not be read
//this is the same as taking the size and subtracting the first index.
const int SpectroHandler::SPECTROGRAM_SIZE = static_cast<int>(
       std::min(std::ceil(MAX_INTERESTING_FREQUENCY * FRAMES_PER_BUFFER / SAMPLE_RATE),FRAMES_PER_BUFFER / 2.0)
            - std::ceil(FRAMES_PER_BUFFER / SAMPLE_RATE * START_FREQUENCY));
const int SpectroHandler::TIMES_SAMPLED = static_cast<int>(SAMPLE_RATE) * RECORDING_TIME / FRAMES_PER_BUFFER + 1;

//total number of samples saved/displayed in spectrogram. In other words, this is the area of the image file
//or equivalent to the number of samples multiplied by the number of buckets saved in the spectrogram
const int SpectroHandler::TOTAL_SAMPLES = SPECTROGRAM_SIZE * TIMES_SAMPLED;

streamCallbackData* SpectroHandler::spectrogramData;
uint8_t* SpectroHandler::spectrogramMagnitudeHistory;

int SpectroHandler::currentMagnitudeIndex = 0;

/*
* Allocates memory for spectrogramData, and its long input and output arrays, using fftw_malloc() for the arrays.
* If those arrays fail to allocate memory, and error will be thrown. Also, this function
* initializes everything which has to do with spectrogramData. The plan for this fftw uses FFTW_MEASURE in a discrete
 * fourier transform real-to-complex 1-dimensional plan, adding it to the spectrogramData struct.
 */
void SpectroHandler::initializeSpectrogramData() {
    spectrogramData = static_cast<streamCallbackData *>(malloc(sizeof(streamCallbackData)));

    //only the left channel is used
    spectrogramData->in = static_cast<double *>(fftw_malloc( sizeof(double) * FRAMES_PER_BUFFER));
    spectrogramData->out = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * FRAMES_PER_BUFFER));
    if (spectrogramData->in == nullptr || spectrogramData->out == nullptr) {
        printf("Could not allocate spectrogram data\n");
        exit(EXIT_FAILURE);
    }

    //creates the plan to be used for fftw
    spectrogramData->p = fftw_plan_dft_r2c_1d
            (FRAMES_PER_BUFFER, spectrogramData->in, spectrogramData->out, FFTW_MEASURE);

    //find start and end indexes on spectrogramData
    constexpr double secondsPerBuffer = FRAMES_PER_BUFFER / SAMPLE_RATE;
    spectrogramData->startIndex = std::ceil(secondsPerBuffer * START_FREQUENCY);
    spectrogramData->spectrogramSize = SPECTROGRAM_SIZE;
}

/*
 * allocates memory for spectrogramMagnitudeData, which is used for further 3d plotting in a saved file.
 * MUST BE CALLED AFTER initializeSpectrogramData()
 *
 * if recording for 10 seconds, the full spectrogram uses 195 kilobytes of data,
 * if recording for 10 seconds, saving only piano frequencies uses 17.2 kilobytes of data
 * slightly more data is used if recording at 48000 hz
 */

void SpectroHandler::allocateMagnitudeHistoryMemory(const bool saveKeysOnly) {
    if (!saveKeysOnly)spectrogramMagnitudeHistory = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * TOTAL_SAMPLES));

    else spectrogramMagnitudeHistory = static_cast<uint8_t*>
            (malloc(sizeof(uint8_t) * 88 * static_cast<int>(SAMPLE_RATE) * RECORDING_TIME / FRAMES_PER_BUFFER));

    if (spectrogramMagnitudeHistory == nullptr) {
        printf("Could not allocate memory for spectrogramMagnitudeHistory.");
        exit(EXIT_FAILURE);
    }
}

/*
 * Uses fftw_free() to free up memory from spectrogramData->( plan, input array, output array) and then frees up memory
 * from spectrogramData itself with free(). Should be called at the end of the use of spectrogramData, obviously.
 */
void SpectroHandler::deallocateSpectrogramData() {
    fftw_destroy_plan(spectrogramData->p);
    fftw_free(spectrogramData->in);
    fftw_free(spectrogramData->out);

    free(spectrogramData);
}

//Deallocates spectrogramMagnitudeHistory. MUST BE CALLED AFTER allocateMagnitudeHistoryMemory()
void SpectroHandler::deallocateMagnitudeHistoryMemory() {
    free(spectrogramMagnitudeHistory);
}

/*
 * function is meant to be called within a portaudio callback function. This assumes that spectrogramMagnitudeHistory
 * is initialized, things will get messed up very badly if this is not the case.
 *
 * This function saves the current spectrogramData -> out to the spectrogramMagnitudeHistory for the length
 * of one buffer, automatically converting it to a byte.
 * */
void SpectroHandler::saveSpectroData() {
    const int lastIndex = spectrogramData -> spectrogramSize + spectrogramData -> startIndex;
    for (int i = spectrogramData -> startIndex; i < lastIndex; i++) {
        if (currentMagnitudeIndex >= TOTAL_SAMPLES) break; //leave the loop if an index out of bounds error would occur

        spectrogramMagnitudeHistory[currentMagnitudeIndex] = shortenDoubleToByte(getMagnitudeAt(i));
        currentMagnitudeIndex++;
    }


}

/*
 * returns the magnitude of spectrogramData at the desired index. Should be called after fft has been performed on
 * spectrogramData, so the information actually means something.
 */
double SpectroHandler::getMagnitudeAt(const int index) {
    const double *complex = spectrogramData->out[index];

    double indexMultiplier = 1;

    if constexpr (AMPLIFY_HIGH_FREQUENCIES) {
        constexpr double maxScaleBy = 20;
        const double slope = (maxScaleBy - 1)/(SPECTROGRAM_SIZE - 1);
        indexMultiplier = slope * (index - 1) + 1;
    }

    return std::hypot(complex[0],complex[1]) * SCALING_FACTOR * indexMultiplier / FRAMES_PER_BUFFER;
}

//converts in from a double to an uint8_t. Linearly scales vales from values ~0-1 to 0-255 (ints only)
//if input values are above 1, the values will not roll over, but will be maxed out at 255
//technically still doesn't work if the double in overflows, but this is unrealistic since numbers will not get that high
uint8_t SpectroHandler::shortenDoubleToByte(const double in) {
    return static_cast<uint8_t>(std::min(255.,in * 255));
}

double SpectroHandler::convertToFrequency(const int index) {
    return index * SAMPLE_RATE / FRAMES_PER_BUFFER;
}

int SpectroHandler::convertToSpectroIndex(const float frequency) {
    return static_cast<int>(frequency * FRAMES_PER_BUFFER / SAMPLE_RATE);
}
