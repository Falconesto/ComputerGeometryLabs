#ifndef INC_5_PICTURE_H
#define INC_5_PICTURE_H


#include <string>
#include <vector>

enum Space {
    YCbCr601,
    RGB
};

struct Pix {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

enum action {
    RGB_with = 0,
    YCBCR_with = 1,
    RGB_classic = 2,
    YCBCR_classic = 3,
    RGB_AUTO = 4,
    YCBCR_AUTO = 5
};


class Picture {
public:
    Space type=RGB;
    void input(std::string filename);
    void output(std::string filename);
    void second_step(double diff, double mult);
    void first_step(bool type);
    void from_YCbCr601_to_RGB();
    void from_RGB_to_YCbCr601();
private:
    int height=0;
    int width=0;
    Pix *data;
    std::string format;
    int colour;
    std::vector<int> get_min_and_max(bool if_r_or_y);
    bool color;
};


#endif //INC_5_PICTURE_H
