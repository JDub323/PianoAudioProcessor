//
// Created by jwhal on 3/1/2025.
//

#ifndef NOTEFILEHANDLER_H
#define NOTEFILEHANDLER_H
#include <cstdint>
#include <string>


class NoteFileHandler {
public:

    static void recordNotesAndSaveInFileProgram();

private:
    static void initializeMyProgramData();
    static bool noteFileExists(const std::string& fileName);
    static std::string calcFileName(int note, int strength);
    //SAVES NOTE IN CUSTOM FILE FORMAT:
    //the format is as follows:
    //the piano note (as an integer), whitespace, the strength as an integer, a newline
    //the note size as an integer (in bytes), whitespace, number of samples in the magnitude spectrogram, whitespace,
    //the first index on the spectrogram for the magnitude array, whitespace, the total number of ticks to be added, newline,
    //samples per second, whitespace, and samples per buffer, followed by a newline character. The raw binary data will follow.
    static void saveNote(int note, int strength, int noteSize, double samplesPerSecond, int samplesPerBuffer,
        int spectroWidth, int spectroFirstIndex, int numTicks, const uint8_t* magnitudeArr);//all other variables are constant, so no need to ask for them
    static void printAllUnsavedNotes();
    static void findFirstUnsavedNote(int &note);
    static void firstUnsavedVolumeAtNote(int note, int &vol);//TODO: Change function def to not use pointers to ints
    static bool noteExistsWithCurrentSettings(int note, int vol);//TODO: make the function check for settings
};



#endif //NOTEFILEHANDLER_H
