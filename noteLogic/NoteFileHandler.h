//
// Created by jwhal on 3/1/2025.
//

#ifndef NOTEFILEHANDLER_H
#define NOTEFILEHANDLER_H



class NoteFileHandler {
public:

    static void recordNotesAndSaveInFileProgram();

private:
    static void initializeMyProgramData();
    static bool noteFileExists();
    static void saveNote();
    static void printAllUnsavedNotes();
    static void findFirstUnsavedNote();
    static void firstUnsavedVolumeAtNote(int note);

};



#endif //NOTEFILEHANDLER_H
