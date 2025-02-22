//
// Created by jwhal on 2/22/2025.
//
//this class is designed to make an object which will save all the times a key was pressed, and at the

#ifndef NOTEHISTORY_H
#define NOTEHISTORY_H
#include <stack>

#define NEVER_TURNED_ON -10000;

//saves time data (what tick the change occurred on) and saves the amplitude of that change
//(soft vs loud press, slight vs total end of the key) NEED TO STANDARDIZE THIS METRIC
//use + to denote increase in frequency (pressing key), negative to denote decrease in frequency (the damper touching the string)
typedef struct {
    int tick;       //can be on or off
    float amplitude;//MAY CHANGE DATA TYPE
} keyStroke;

//stores the
typedef struct {
    keyStroke onStroke;
    keyStroke offStroke;
} keyTickData;

class NoteHistory {
public:
    NoteHistory() {
        for (auto & i : noteHistory) {
            keyStroke null = {0,-1};
            i.push(null);
        }

        //whatever else is needed in initialization
    }

    //returns whether a note is actually on. Checks the
    bool isOn(int key);

    //returns the most recent on press and off press of a key. To be used by the Piano Logic class with NoteData to
    //return a frequency at a point in time
    //
    keyTickData getNote(int key);

    //adds a note to the saved history. Will also add ending notes to save history if amplitude is negative
    void addNote(int key, int tick, float amplitude);



private:
    //array of stacks to access the most recent
    std::stack<keyStroke> noteHistory[88];

};



#endif //NOTEHISTORY_H
