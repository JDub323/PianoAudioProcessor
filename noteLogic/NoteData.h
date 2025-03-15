//
// Created by jwhal on 2/21/2025.
//

#ifndef NOTE_H
#define NOTE_H
#include <cstdio>


class Note {
public:
    explicit Note(const int index) : noteNumber(index) {//explicit no longer necessary if another parameter is added
        printf("Constructor called, index is %d\n", noteNumber);
        //consider, if no file of precalculated data is saved, adding the feature to tune the tool for the piano here

        //will malloc precalculated data to return a pointer to a vector in 927 space from two parameters, dynamics
        //and ticks (time) since the note was depressed

        //also need some timestamp array to remember on what tick the note was last depressed and last played
        //OR maybe just two variables for that, but that doesn't save history...
    }
    ~Note() {
        printf("Destructor called");
    }
    float* getFreqDistribution(int tickValue);  //TODO (this is an object which makes up the note history class)

private:
    int noteNumber;

};



#endif //NOTE_H
