//
// Created by jwhal on 11/19/2024.
//

#include "PianoLogic.h"

#include <array>



std::string PianoLogic::calcKeyString(const int pianoKey) {
    if (pianoKey < 0 || pianoKey > 87) {
        return "Invalid key number";
    }

    const std::string notes[] = {"A ", "A#", "B ", "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#"};
    const int octave = (pianoKey + 9) / 12;
    const int noteIndex = pianoKey % 12;

    return notes[noteIndex] + std::to_string(octave);
}


