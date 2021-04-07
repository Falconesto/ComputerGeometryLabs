//
// Created by Александр on 11.06.2020.
//

#ifndef INC_4_IMAGE_H
#define INC_4_IMAGE_H


#include <iostream>
#include <cmath>

using namespace std;

struct myInput {
    int from;
    int to;
    int countInput;
    int countOutput;
    char *inputFile;
    char *outputFile;
};

struct Pixel {
    double red;
    double green;
    double blue;
};

struct fileHeader {
    FILE *file;
    int format;
    int width;
    int height;
    int colors;
    int size;
};

class Image {
public:
    void readOneFile(myInput fileData);

    void readThreeFiles(myInput fileData);

    void writeOneFile(myInput fileData);

    void writeThreeFiles(myInput fileData);

    void action(myInput newInput);

    ~Image();
private:
    fileHeader *data;
    Pixel *pixelData;

    void RGBtoHSL();

    void RGBtoHSV();

    void RGBtoYCbCr601();

    void RGBtoYCbCr709();

    void RGBtoYCoCg();

    void RGBtoCMY();

    void HSLtoRGB();

    void HSVtoRGB();

    void YCbCr601toRGB();

    void YCbCr709toRGB();

    void YCoCgtoRGB();

    void CMYtoRGB();
};


#endif //INC_4_IMAGE_H
