//
// Created by jwhal on 3/1/2025.
//

#include "NoteFileHandler.h"

#include <conio.h>
#include <cstdio>
#include <portaudio.h>
#include <filesystem>
#include <fstream>

#include "PianoLogic.h"
#include "../myAudio/AudioFinder.h"
#include "../myAudio/CallbackFunctions.h"

#include "SpectroHandler.h"

#define PATH_TO_DATABASE "../noteDatabase/"

NoteProgramData* NoteFileHandler::myProgramData;

void NoteFileHandler::recordNotesAndSaveInFileProgram() {
    printf("This program will save the piano notes recorded in a file folder.\n");
    printf("Initializing...\n");

    if constexpr (RECORDING_TIME != 10) {
        printf("Recording time incorrect. Exiting function\n");
        return;
    }

    AudioFinder::initializePortAudio();

    //initialize global variable spectrogramData
    SpectroHandler::initializeSpectrogramData();

    SpectroHandler::allocateMagnitudeHistoryMemory(false);//allocate memory to save magnitude history

    initializeMyProgramData();

    printf("Successfully Initialized. Checking for notes which have not been saved.\n");

    //TODO: check for notes which have not been saved, set them to nextNote
    //for now, I will have it hard coded
    int nextNote = 0;
    int onVolume = VOL_FF;

    while (nextNote < 88 && onVolume >= VOL_PP) {

        printf("To begin recording at key %s and volume %s, press 'r'\n",
            PianoLogic::calcKeyString(nextNote).c_str(), PianoLogic::calcVolumeString(onVolume).c_str());
        printf("To see which notes have already been saved, press 's'\n");
        printf("To exit the program, press 'e'\n");

        while (true) {
            if (_getch() == 'r') {
                break;
            }
            Pa_Sleep(100);
        }

        printf("\nWaiting...");

        Pa_Sleep(1000);//wait for the noise from the keystroke to dissipate before listening for a volume spike

        AudioFinder::startRecording(CallbackFunctions::listenForSingleNote, myProgramData);

        printf("\r");
        printf("Ready for note. Play %s at %s volume",
            PianoLogic::calcKeyString(nextNote).c_str(), PianoLogic::calcVolumeString(onVolume).c_str());

        //this way of sleeping may cause an extra buffer or two to be run. Hopefully not an issue
        //if I save magnitudeHistory correctly (that is, disallow extra buffers to be added) things will be fine probably
        while(myProgramData -> stillRecording) {
            Pa_Sleep(50);   //lowered millis I sleep for
        }

        AudioFinder::pausePortAudio();

        //check to see if the note was played well enough to pass (sufficient volume and harmonics that are at least close)
        //if the note is good enough, save the file and move on, otherwise try again
        //add a skip note option for the options given in the beginning in case not ready

        //save file around here (and let user know the file was saved successfully)


        nextNote++;
        onVolume -= 2;//change these

        //check if the next note has been saved, or maybe that is already done above so there is no need. IDRK

    }


    AudioFinder::quitAndDeallocate();

    SpectroHandler::deallocateSpectrogramData();



    SpectroHandler::deallocateMagnitudeHistoryMemory();


    printf("When ready to begin, press \'r\'");

}

void NoteFileHandler::initializeMyProgramData() {
    //TODO: may be wrong since allocated to the stack (potentially) instead of the heap
    myProgramData = new NoteProgramData{1,  //set to 1 so the first volume read is not interpreted as a major increase
        SpectroHandler::spectrogramData, //this will always be the global var. Should it have been a global var? no
    true};  //it is true that it is still recording
}

bool NoteFileHandler::noteFileExists(const std::string& fileName) {
    return std::filesystem::exists(PATH_TO_DATABASE + fileName);
}

std::string NoteFileHandler::calcFileName(const int note, const int strength) {
    return PianoLogic::calcKeyString(note) + PianoLogic::calcVolumeString(strength) + ".dat";
}


void NoteFileHandler::saveNote(const AudioSettings settings, const int note,
    const int strength,
    const int noteSize, const int numTicks, const uint8_t* magnitudeArr) {

    const std::string fileName = calcFileName(note, strength);

    //make and open file
    std::ofstream file(PATH_TO_DATABASE + fileName, std::ios::out);
    if (!file) {//TODO: check to see if this is the way to exit from this function
        printf("Error opening file");
        SpectroHandler::deallocateMagnitudeHistoryMemory();
        exit(EXIT_FAILURE);
    }

    //write beginning formatting to file
    file << note << " " << strength << " " << noteSize << " " << numTicks << "\n"
    << settings.spectroWidth << " " << settings.spectroFirstIndex << " "
    << settings.samplesPerSecond << " " << settings.samplesPerBuffer << "\n";

    file.close();
    file.open(fileName, std::ios::out | std::ios::app | std::ios::binary);

    //check to see if there was another error opening the file
    if (!file) {//check here too
        printf("Error opening file");
        SpectroHandler::deallocateMagnitudeHistoryMemory();
        exit(EXIT_FAILURE);
    }

    file.write(reinterpret_cast<const char *>(magnitudeArr), settings.spectroWidth * numTicks * noteSize);
}

bool NoteFileHandler::noteExistsWithCurrentSettings(const int note, const int strength) {
    FILE* fptr = fopen(calcFileName(note,strength).c_str(), "r");
    if (fptr == nullptr) {
        //file with such a name and strength does not exist, therefore, return false
        fclose(fptr);
        return false;
    }

    return audioSettingsMatch(getAudioSettings(fptr), AudioFinder::getAudioSettings());
}

//Assume the file pointer is opened on read mode
AudioSettings NoteFileHandler::getAudioSettings(FILE *fptr) {
    //skip the first line, not useful information
    char buffer[20];
    fgets(buffer, 20, fptr);    //skip the first line which contains irrelevant information

    constexpr AudioSettings ret = {};   //another possible struct error here

    fscanf(fptr, "%d", &ret.spectroWidth);
    fscanf(fptr, "%d", &ret.spectroFirstIndex);
    fscanf(fptr, "%d", &ret.samplesPerSecond);
    fscanf(fptr, "%d", &ret.samplesPerBuffer);

    return ret;
}

bool NoteFileHandler::audioSettingsMatch(const AudioSettings a, const AudioSettings b) {
    if (a.spectroWidth != b.spectroWidth) return false;
    if (a.samplesPerBuffer != b.samplesPerBuffer) return false;
    if (a.samplesPerSecond != b.samplesPerSecond) return false;
    if (a.spectroFirstIndex != b.spectroFirstIndex) return false;

    return true;
}
