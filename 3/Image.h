//
// Created by Александр on 07.06.2020.
//

#ifndef INC_3_IMAGE_H
#define INC_3_IMAGE_H

#ifndef CompGraph3_PICTURE_H
#define CompGraph3_PICTURE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const double orderedTable[8][8] = {
        {0.0 / 64.0, 48.0 / 64.0, 12.0 / 64.0, 60.0 / 64.0, 3.0 / 64.0, 51.0 / 64.0, 15.0 / 64.0, 63.0 / 64.0},
        {32.0 / 64.0, 16.0 / 64.0, 44.0 / 64.0, 28.0 / 64.0, 35.0 / 64.0, 19.0 / 64.0, 47.0 / 64.0, 31.0 / 64.0},
        {8.0 / 64.0, 56.0 / 64.0, 4.0 / 64.0, 52.0 / 64.0, 11.0 / 64.0, 59.0 / 64.0, 7.0 / 64.0, 55.0 / 64.0},
        {40.0 / 64.0, 24.0 / 64.0, 36.0 / 64.0, 20.0 / 64.0, 43.0 / 64.0, 27.0 / 64.0, 39.0 / 64.0, 23.0 / 64.0},
        {2.0 / 64.0, 50.0 / 64.0, 14.0 / 64.0, 62.0 / 64.0, 1.0 / 64.0, 49.0 / 64.0, 13.0 / 64.0, 61.0 / 64.0},
        {34.0 / 64.0, 18.0 / 64.0, 46.0 / 64.0, 30.0 / 64.0, 33.0 / 64.0, 17.0 / 64.0, 45.0 / 64.0, 29.0 / 64.0},
        {10.0 / 64.0, 58.0 / 64.0, 6.0 / 64.0, 54.0 / 64.0, 9.0 / 64.0, 57.0 / 64.0, 5.0 / 64.0, 53.0 / 64.0},
        {42.0 / 64.0, 26.0 / 64.0, 38.0 / 64.0, 22.0 / 64.0, 41.0 / 64.0, 25.0 / 64.0, 37.0 / 64.0, 21.0 / 64.0},
};

const double JarvisTable[3][5] = {
        {0, 0, 0, 7.0 / 48.0, 5.0 / 48.0},
        {3.0 / 48.0, 5.0 / 48.0, 7.0 / 48.0, 5.0 / 48.0, 3.0 / 48.0},
        {1.0 / 48.0, 3.0 / 48.0, 5.0 / 48.0, 3.0 / 48.0, 1.0 / 48.0}
};

const double SIERRATable[3][5] = {
        {0, 0, 0, 5.0 / 32.0, 3.0 / 32.0},
        {2.0 / 32.0, 4.0/ 32.0, 5.0 / 32.0, 4.0 / 32.0, 2.0 / 32.0},
        {0, 2.0 / 32.0, 3.0 / 32.0, 2.0 / 32.0, 0}
};

const double ATKINSONTable[3][4] = {
        {0, 0, 1, 1},
        {1, 1, 1, 0},
        {0, 1, 0, 0}
};

const double halftoneTable[4][4] = {
        {7.0 / 17.0, 13.0 / 17.0, 11.0 / 17.0, 4.0 / 17.0},
        {12.0 / 17.0, 16.0 / 17.0, 14.0 / 17.0, 8.0/17.0},
        {10.0 / 17.0, 15.0 / 17.0, 6.0 / 17.0, 2.0 / 17.0},
        {5.0 / 17.0, 9.0 / 17.0, 3.0 / 17.0, 1.0 / 17.0},
};


enum algorithm {
    No = 0,
    ordered = 1,
    random = 2,
    floyd_stein = 3,
    jarvis = 4,
    sierra = 5,
    atkinson = 6,
    halftone = 7
};

class Image {
public:
    explicit Image(FILE *file);

    void printInFile(FILE *outputFile);

    void createGradient();

    void dither(algorithm algo, unsigned char bitNumber);

    void setGamma(double g);
    
private:
    short format;
    int width;
    int height;
    int colors;
    double gamma;
    unsigned char *data;

    int new_bit_colour(int x, int bits);

    unsigned char& get(int h, int w);

    void None(unsigned char bitRate);

    void Ordered(unsigned char bitRate);

    void Random(unsigned char bitRate);

    void Floyd(unsigned char bitRate);

    void Jarvis(unsigned char bitRate);

    void Sierra(unsigned char bitRate);

    void Atkinson(unsigned char bitRate);

    void Halftone(unsigned char bitRate);

    double gammaFromArrayToPicture(double color);

    double& get_error (vector<double> &errors, int h, int w );

    double gammaFromPictureToArray(double color);
};
#endif


#endif //INC_3_IMAGE_H
