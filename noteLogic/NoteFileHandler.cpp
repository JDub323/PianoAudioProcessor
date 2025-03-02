//
// Created by jwhal on 3/1/2025.
//

#include "NoteFileHandler.h"

#include <conio.h>
#include <cstdio>
#include <portaudio.h>

#include "PianoLogic.h"
#include "../myAudio/AudioFinder.h"
#include "../myAudio/CallbackFunctions.h"

#include "SpectroHandler.h"

typedef struct {
    double previousVolume = 1;  //set to 1 so the first volume read is not interpreted as a major increase
    streamCallbackData* spectroData = SpectroHandler::spectrogramData;  //this will always be the global var. Should it have been a global var? no
    bool stillRecording = true;

} noteProgramData;

noteProgramData* myProgramData;

void NoteFileHandler::recordNotesAndSaveInFileProgram() {
    printf("This program will save the piano notes recorded in a file folder.\n");
    printf("Initializing...\n");

    if constexpr (RECORDING_TIME != 10) {
        printf("Recording time incorrect. Exiting function\n");
        return;
    }

    AudioFinder::connectToMic();

    //initialize global variable spectrogramData
    SpectroHandler::initializeSpectrogramData();

    SpectroHandler::allocateMagnitudeHistoryMemory(false);//allocate memory to save magnitude history

    initializeMyProgramData();

    printf("Successfully Initialized. Checking for notes which have not been saved.\n");

    //TODO: check for notes which have not been saved, set them to nextNote
    //for now, I will have it hard coded
    int nextNote = 0;
    int onVolume = VOL_FF;

    while (nextNote < 88 && onVolume > VOL_PP) {

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

        Pa_Sleep(1000);//wait for the noise from the key to dissipate before listening for a volume spike

        AudioFinder::startRecording(CallbackFunctions::listenForSingleNote, myProgramData);

        printf("\r");
        printf("Ready for note. Play %s at %s volume",
            PianoLogic::calcKeyString(nextNote).c_str(), PianoLogic::calcVolumeString(onVolume).c_str());

        //this way of sleeping may cause an extra buffer or two to be run. Hopefully not an issue
        //if I save magnitudeHistory correctly (that is, disallow extra buffers to be added) things will be fine probably
        while(myProgramData -> stillRecording) {
            Pa_Sleep(100);
        }

        AudioFinder::pauseRecording();

        //check to see if the note was played well enough to pass (sufficient volume and harmonics that are at least close)
        //if the note is good enough, save the file and move on, otherwise try again
        //add a skip note option for the options given in the beginning in case not ready

        //save file around here (and let user know the file was saved successfully

        nextNote++;
        onVolume -= 2;//change these

        //check if the next note has been saved, or maybe that is already done above so there is no need. IDRK.
    }


    AudioFinder::micQuitAndDeallocate();

    SpectroHandler::deallocateSpectrogramData();



    SpectroHandler::deallocateMagnitudeHistoryMemory();


    printf("When ready to begin, press \'r\'");

}
