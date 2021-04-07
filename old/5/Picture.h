//
// Created by Александр on 04.06.2020.
//

#ifndef INC_5_PICTURE_H
#define INC_5_PICTURE_H


#include <string>
#include <vector>


struct Pixel {
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
};


enum Space {
    YCbCr601,
    RGB
};


enum Conversion {
    RGB_with = 0,
    YCBCR_with = 1,
    RGB_classic = 2,
    YCBCR_classic = 3,
    RGB_AUTO = 4,
    YCBCR_AUTO = 5
};


class Picture {
public:

    Picture();

    ~Picture();

    void Open_File(std::string);

    void Save_File(std::string);

    void whichType(Space);

    void changeBrightness(double difference, double multiple);

    void Brightness(bool type);

    void YToR();

    void RToY();

private:
    int height;
    int width;
    Pixel *image;
    Space type;
    std::string format;
    int colour;
    std::vector<int> getBrightestAndDarkest(bool with_out);
    bool color;
};


#endif //INC_5_PICTURE_H
