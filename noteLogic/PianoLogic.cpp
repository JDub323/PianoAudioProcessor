//
// Created by jwhal on 11/19/2024.
//

#include "PianoLogic.h"

#include <array>
#include <cmath>

std::string PianoLogic::topFivePeaksString(const float *arr, const int size) {
    int peakIndexes[5] = {0};
    float peakVals[5] = {0};

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 5; j++) {
            if (arr[i] > peakVals[j]) {
                peakIndexes[j] = i;
                peakVals[j] = arr[i];
                break;
            }
        }
    }

    std::string ret = "Top Five Peaks: ";

    for (const int index : peakIndexes) {
        const int peak = getPianoIndexClosestTo(index);
        ret += calcKeyString(peak);
        ret += " ";
    }

    return ret;
}


std::string PianoLogic::calcKeyString(const int pianoKey) {
    if (pianoKey < 0 || pianoKey > 87) {
        return "Invalid key number";
    }

    const std::string notes[] = {"A ", "A#", "B ", "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#"};
    const int octave = (pianoKey + 9) / 12;
    const int noteIndex = pianoKey % 12;

    return notes[noteIndex] + std::to_string(octave);
}



//estimates the piano index that is closest to the frequency given
//is not perfect since there is logarithmic scaling for piano keys,
//so higher notes will be favored more than they should
int PianoLogic::getPianoIndexClosestTo(const int frequency) {
    constexpr double A0freq = 27.5;

    return static_cast<int>(std::round(12 * log2(frequency / A0freq)));
}


