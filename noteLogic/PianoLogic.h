//
// Created by jwhal on 11/19/2024.
//

//This class handles piano logic, using algorithms to give the correct

//piano keys are 0 indexed, little endian, so a0 = 0, a0# = 1, ... ,  b7 = 86, c8 = 87
#ifndef PIANOLOGIC_H
#define PIANOLOGIC_H
#include <cstdint>
#include <string>
#include <vector>

class PianoLogic {
public:
    static std::string topFivePeaksString(const double* arr, int size, int searchStartIndex);
    static std::string calcKeyString(int pianoKey);
    //takes the riemann sum and normalizes the result on a standard scale.
    //normalizes the result (finds the average value instead of the sum of frequencies)
    //note that the getMagnitudeAt function of SpectroHandler also normalizes the sum, which
    //is not default behavior for fftw3
    static double getVolume(const double* arr, int size);

    static void testPianoIndexFinder();

private:
    //sums all values in the array. To be used by getVolume
    //be careful when using this to give a pointer to the first value which you want summed and the length of the
    //array you want summed
    static double riemannSum(const double* arr, int size);
    static int getPianoIndexClosestTo(double frequency);
};



#endif //PIANOLOGIC_H


/*
 * TODO:
 *
 *      standardize piano note history array:
 *          bool array with 1 where a note was played
 *          amplitude of each note at time t
 *          linked list of notes each with start, end, dynamic
 *          array of notes, history for each note  <<----- CHOOSING THIS ONE AS OF NOW
 *              *actually is implemented as an array of stacks of noteData (a struct)
 *
 *
 *      real-time peak finder DONE
 *
 *
 *      piano music synthesizer
 *          consider using some standard format for digital music found online
 *          or add an individual note manually in some class, go to frequency space, then reverse fft
 *          STEPS:
 *              1 standard note type
 *              2 saved data on note deprecation
 *                    use a real-time Riemann sum to gauge how notes lose volume then write a
 *                    scaling factor that normalizes a pgm image for volume to see if frequency
 *                    distribution changes as the volume diminishes
 *
 *              3 saved data on frequency changes with changes in dynamics
 *                    consider finding an equation to calculate this, but having it precalculated could be faster (chess programming lessons)
 *
 *              4 standard note builder (notes --> frequency-time-amplitude array (or array that varies by the tickValue))
 *                    For this I would need a getFreqDistribution(int tickValue) method for a note class (OOP incoming)
 *                          this method would use a history of being pressed at some hardness custom for that note and
 *                          the history of being raised for that same note, and returns how loud it should be at that note
 *
 *          DO NEXT 2 EARLIER
*              5 freq-time-amplitude array --> .wav data (reverse fft)
 *                      This array must be, seemingly, half the count and also complex numbers (fftw_half complex to real)
*
 *             6 .wav data played on speakers through portaudio
 *
 *
 */

//what is interesting to think about is how the frequency vector changes depending on dynamics and time since depression
//I think it is just a vector in 3-space...? I need to study that space to understand if there are any shortcuts
//I can take

//It is similar to the pgm image 3-space vector

//NVM I FORGOT ABOUT AMPLITUDE, it is a colored function in 3 space

//it could also be thought of as a transformation going from R2 --> R927 (or whatever the spectro width is). Is it linear??