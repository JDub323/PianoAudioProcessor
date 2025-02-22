//
// Created by jwhal on 1/28/2025.
//

#ifndef SPECTROIMAGEMAKER_H
#define SPECTROIMAGEMAKER_H
#include <cstdint>
#include <iosfwd>


class SpectroImageMaker {
public:
    static void makePGMImage(const uint8_t *magnitudeHistory, int width, int height, bool savePianoKeysOnly, const std::string &fileName);


};



#endif //SPECTROIMAGEMAKER_H
