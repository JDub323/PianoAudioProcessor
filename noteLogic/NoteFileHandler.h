//
// Created by jwhal on 3/1/2025.
//

#ifndef NOTEFILEHANDLER_H
#define NOTEFILEHANDLER_H
#include <cstdint>
#include <string>

#include "../myAudio/AudioFinder.h"

typedef struct {
    double previousVolume;
    streamCallbackData* spectroData;
    bool stillRecording;

} NoteProgramData;


class NoteFileHandler {
public:

    static void recordNotesAndSaveInFileProgram();
    static AudioSettings getAudioSettings(FILE* fptr);
    static bool audioSettingsMatch(AudioSettings a, AudioSettings b);
    static NoteProgramData* myProgramData;


private:
    static void initializeMyProgramData();
    static bool noteFileExists(const std::string& fileName);
    static std::string calcFileName(int note, int strength);
    //SAVES NOTE IN CUSTOM FILE FORMAT:
    //the format is as follows:
    //the piano note (as an integer), whitespace, the strength as an integer, whitespace, the note size as an integer (in bytes), whitespace,
    //the total number of ticks to be added, newline,
    //number of samples in the magnitude spectrogram, whitespace, the first index on the spectrogram for the magnitude array, whitespace,
    //samples per second, whitespace, and samples per buffer, followed by a newline character. The raw binary data will follow.
    static void saveNote(AudioSettings settings, int note, int strength, int noteSize, int numTicks, const uint8_t* magnitudeArr);//all other variables are constant, so no need to ask for them
    static void printAllUnsavedNotes();
    static void findFirstUnsavedNote(int &note);
    static int getFirstUnsavedVolume(int note);//returns a volume
    static bool noteExistsWithCurrentSettings(int note, int strength);

};



#endif //NOTEFILEHANDLER_H
