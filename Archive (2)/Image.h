//
// Created by Nikolay Rulev on 17.03.2020.
//

#ifndef INC_2_IMAGE_H
#define INC_2_IMAGE_H

#include <string>
#include <vector>
#include "Args.h"

using namespace std;

class Image {
public:
    Image() = default;
    void read(const string &input);
    void read(const string &input1, const string &input2, const string &input3);
    void write(const string &output);
    void write(const string &output1, const string &output2, const string &output3);
    unsigned int height() {return height_;}
    unsigned int width() {return width_;}
    void convert(const string &from, const string &to);
private:
    void hsl_rgb();
    void hsv_rgb();
    void ycbcr601_rgb();
    void ycbcr709_rgb();
    void ycocg_rgb();
    void cmy_rgb();
    void rgb_hsl();
    void rgb_hsv();
    void rgb_ycbcr601();
    void rgb_ycbcr709();
    void rgb_ycocg();
    void rgb_cmy();
private:
    vector < vector<double>> buffer[3];
    unsigned int height_{};
    unsigned int width_{};
};


#endif //INC_2_IMAGE_H
