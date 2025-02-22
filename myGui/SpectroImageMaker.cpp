//
// Created by jwhal on 1/28/2025.
//

#include "SpectroImageMaker.h"

#include <cinttypes>
#include <cstdint>
#include <fstream>

#include "../myAudio/AudioFinder.h"
#include "../noteLogic/SpectroHandler.h"


void SpectroImageMaker::makePGMImage(const uint8_t *magnitudeHistory, const int width, const int height, const bool savePianoKeysOnly, const std::string& fileName) {
    //ensure magnitudeHistory has been initialized
    if (!magnitudeHistory) {
        printf("Error: magnitudeHistory is NULL\n");
        SpectroHandler::deallocateMagnitudeHistoryMemory();
        exit(EXIT_FAILURE);
    }

    if (width * height != SpectroHandler::currentMagnitudeIndex) {
        printf("Error: wrong number of samples.\n");
        printf("Want: %d\n", width * height - 1);
        printf("Got: %d\n", SpectroHandler::currentMagnitudeIndex);

        if (width * height < SpectroHandler::currentMagnitudeIndex) {
            printf("Too many samples. Shutting down.\n");
            SpectroHandler::deallocateMagnitudeHistoryMemory();
            exit(EXIT_FAILURE);
        }

        //else
        printf("Too few samples. This is fine (copium)\n");
    }

    //make and open file
    std::ofstream file(fileName, std::ios::out);
    if (!file) {
        printf("Error opening file");
        SpectroHandler::deallocateMagnitudeHistoryMemory();
        exit(EXIT_FAILURE);
    }

    //write beginning formatting to file
    file << "P5\n"
    << width << " " << height << "\n"
    << "255\n";

    file.close();
    file.open(fileName, std::ios::out | std::ios::app | std::ios::binary);

    //check to see if there was another error opening the file
    if (!file) {
        printf("Error opening file");
        SpectroHandler::deallocateMagnitudeHistoryMemory();
        exit(EXIT_FAILURE);
    }

    //write contents to file
    if (savePianoKeysOnly) {
        //do magic
    }
    else {
        file.write(reinterpret_cast<const char *>(magnitudeHistory), width * height);
    }

    file.close();
}