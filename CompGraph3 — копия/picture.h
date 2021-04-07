#ifndef CompGraph3_PICTURE_H
#define CompGraph3_PICTURE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
typedef unsigned char uchar;

enum dith_alg {

    No = 0,

    Dith_Ordered = 1,

    Dith_Random = 2,

    Dith_F_S = 3,

    Dith_J_J_N = 4,

    Dith_Sierra = 5,

    Dith_Atkinson = 6,

    Dith_Halftone = 7
};

class Picture {
public:

    Picture();
    void read(string fileName);
    void write(const string& fileName);
    void gradient();
    void dither(dith_alg algo, uchar bitRate);
    void set_gamma(double g);

private:

    short format;
    int width;
    int height;
    int colors;
    double gamma;
    vector<uchar> data;
    uchar& get(int h, int w);
    void None(uchar bitRate);
    void Ordered(uchar bitRate);
    void Random(uchar bitRate);
    void F_S(uchar bitRate);
    void J_J_N(uchar bitRate);
    void Sierra(uchar bitRate);
    void Atkinson(uchar bitRate);
    void Halftone(uchar bitRate);
    uchar undo_color_correction(uchar color);
    double undo_value_correctionSRGB(double k);
    double undo_value_correction(double k);
    double& get_error (vector<double> &errors, int h, int w );
    uchar correct_color(uchar color);
    double correct_value_SRGB(double k);
    double correct_value(double k);
};
#endif
