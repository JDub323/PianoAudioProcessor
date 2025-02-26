//
// Created by jwhal on 11/19/2024.
//

#include "PianoLogic.h"

#include <array>
#include <cmath>

#include "SpectroHandler.h"

std::string PianoLogic::topFivePeaksString(const double *arr, const int size, const int searchStartIndex) {
    int peakIndexes[5];
    double peakVals[5];

    for (int i = searchStartIndex; i < size + searchStartIndex; i++) {
        for (int j = 0; j < 5; j++) {
            if (arr[i] > peakVals[j]) {
                peakIndexes[j] = i;
                peakVals[j] = arr[i];
                break;
            }
        }
    }

    std::string ret = "Top Five Peaks: ";

    for (const int i : peakIndexes) {
        const int peak = getPianoIndexClosestTo(SpectroHandler::convertToFrequency(i));
        ret += calcKeyString(peak);
        ret += " ";
    }

    return ret;
}


std::string PianoLogic::calcKeyString(const int pianoKey) {
    if (pianoKey < 0 || pianoKey > 87) {
        return "Invalid key number";
    }

    const std::string notes[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
    const int octave = (pianoKey + 9) / 12;
    const int noteIndex = pianoKey % 12;

    return notes[noteIndex] + std::to_string(octave);
}

double PianoLogic::getVolume(const double *arr, const int size) {
    const double sum = riemannSum(arr, size);
    constexpr double scalingFactor = 1;
    return scalingFactor * sum / static_cast<double>(SpectroHandler::SPECTROGRAM_SIZE);
}

void PianoLogic::testPianoIndexFinder() {
    for (int i = -20; i < 5000; i += 100) {
        printf("%5d: %d\n", i, getPianoIndexClosestTo(i));
    }


}



double PianoLogic::riemannSum(const double *arr, const int size) {
    double ret = 0;
    for (int i = 0; i < size; i++) {
        ret += arr[i];
    }
    return ret;
}

//estimates the piano index that is closest to the frequency given
//is not perfect since there is logarithmic scaling for piano keys,
//so higher notes will be favored more than they should
int PianoLogic::getPianoIndexClosestTo(const double frequency) {
    constexpr double A0freq = 27.5;

    int ret = static_cast<int>(std::round(12 * log2(frequency / A0freq)));

    ret = std::clamp(ret, 0, 87);

    return ret;
}


