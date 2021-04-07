
#include "Picture.h"
#include <bits/stdc++.h>


Picture::Picture() : height(0), width(0), image(nullptr), type(RGB) {}

Picture::~Picture() {
    delete[] image;
}

void Picture::Open_File(std::string filename) {
    std::ifstream input;
    input.open(filename, std::ios_base::in | std::ios_base::binary);
    input >> format;

    if (format == "P5") {
        color = false;
    } else {
        if (format == "P6") {
            color = true;
        } else {
            std::cerr << "File has wrong format, format must be P5 or P6 , check input file" << std::endl;
        }
    }
    input >> width >> height;
    if ((height < 1) || (width < 1)) {
        std::cerr << "Please check the height or width in the FrameHeader" << std::endl;
    }

    input >> colour;
    if (colour != 255) {
        std::cerr << "Unacceptable format" << std::endl;
    }

    char partPixel;
    char *image_char = &partPixel;
    image = new Pixel[width * height];
    input.read(image_char, 1);

    for (int i = 0; i < width * height; i++) {
        if (color) {
            input.read(image_char, 1);
            image[i].Red = *image_char;

            input.read(image_char, 1);
            image[i].Green = *image_char;

            input.read(image_char, 1);
            image[i].Blue = *image_char;
        } else {
            input.read(image_char, 1);
            image[i].Red = *image_char;
            image[i].Green = *image_char;
            image[i].Blue = *image_char;
        }
    }
    input.close();
}

void Picture::Save_File(std::string filename) {
    std::ofstream output;
    output.open(filename, std::ios_base::out | std::ios_base::binary);

    if (color) {
        output << "P6" << '\n';
    } else {
        output << "P5" << '\n';
    }
    output << width << ' ' << height << '\n' << 255 << '\n';

    char *image_char = (char *) image;
    if (color) {
        output.write(image_char, width * height * 3);
    } else {
        for (int i = 0; i < width * height; i++) {
            output.write(&image_char[i * 3], 1);
        }
    }
    output.close();
}

void Picture::whichType(Space newCS) {
    type = newCS;
}

void Picture::changeBrightness(double difference, double multiple) {
    if (color) {
        if (type == RGB) {
            for (int i = 0; i < width * height; i++) {
                int result = ((double) image[i].Red - difference) * multiple;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                image[i].Red = result;
                result = ((double) image[i].Green - difference) * multiple;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                image[i].Green = result;
                result = ((double) image[i].Blue - difference) * multiple;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                image[i].Blue = result;
            }
        } else {
            for (int i = 0; i < width * height; i++) {
                int result = ((double) image[i].Red - difference) * multiple;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                image[i].Red = result;
            }
        }
    } else {
        if (type == RGB) {
            for (int i = 0; i < width * height; i++) {
                int result = ((double) image[i].Red - difference) * multiple;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                image[i].Red = result;
                image[i].Green = result;
                image[i].Blue = result;
            }
        } else {
            for (int i = 0; i < width * height; i++) {
                int result = ((double) image[i].Red - difference) * multiple;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                image[i].Red = result;
            }
        }
    }
}

void Picture::Brightness(bool type) {
    std::vector<int> get;
    get = getBrightestAndDarkest(type);
    std::cout << get[0] << " " << get[1] << " " << std::endl;
    double difference = (get[1]);
    double multiple = 255.0 / (double) (get[0] - get[1]);
    std::cout << "Difference: " << difference << ".\nMultiple: " << multiple << ".\n";
    changeBrightness(difference, multiple);
}

std::vector<int> Picture::getBrightestAndDarkest(bool with_out) {
    int pixels[256] = {};
    int brightest = 0;
    int darkest = 0;
    for (int i = 0; i < width * height; i++) {
        if ((color) && (type != YCbCr601)) {
            pixels[image[i].Red]++;
            pixels[image[i].Green]++;
            pixels[image[i].Blue]++;
        } else {
            pixels[image[i].Red]++;
        }
    }
    if (with_out) {
        int to_deleted = 0;
        int deleted = 0;
        int Dposition = 0;
        int Bposition = 255;
        if ((color) && (type != YCbCr601)) {
            to_deleted = (double) width * height * 3 * 0.0039;
        } else {
            to_deleted = (double) width * height * 0.0039;
        }
        int x = to_deleted;
        int newDeleted = 0;

        while (deleted < to_deleted) {
            while (pixels[Dposition] == 0) {
                Dposition++;
            }
            pixels[Dposition]--;
            deleted++;
        }

        while (newDeleted < x) {
            while (pixels[Bposition] == 0) {
                Bposition--;
            }
            pixels[Bposition]--;
            newDeleted++;
        }
    }

    for (int i = 0; i < 256; i++) {
        if (pixels[i] > 0) {
            darkest = i;
            break;
        }
    }

    for (int i = 255; i >= 0; i--) {
        if (pixels[i] > 0) {
            brightest = i;
            break;
        }
    }

    return {brightest, darkest};
}

void Picture::RToY() {
    double kr = 0.299, kb = 0.114;
    for (int i = 0; i < width * height; i++) {
        double r = image[i].Red / 255.0;
        double g = image[i].Green / 255.0;
        double b = image[i].Blue / 255.0;
        double y = kr * r + (1 - kr - kb) * g + kb * b;
        double pb = 0.5 * (b - y) / (1.0 - kb);
        double pr = 0.5 * (r - y) / (1.0 - kr);
        image[i].Red = y * 255.0;
        image[i].Green = (pb + 0.5) * 255.0;
        image[i].Blue = (pr + 0.5) * 255.0;
    }
}

void Picture::YToR() {
    double kr = 0.299, kb = 0.114;
    for (int i = 0; i < width * height; i++) {
        double y = image[i].Red / 255.0;
        double pb = image[i].Green / 255.0 - 0.5;
        double pr = image[i].Blue / 255.0 - 0.5;
        double r, g, b;
        b = 2 * pb * (1 - kb) + y;
        r = 2 * pr * (1 - kr) + y;
        g = (y - kr * r - kb * b) / (1 - kr - kb);
        r = r < 0 ? 0 : r;
        r = r > 1 ? 1 : r;
        b = b < 0 ? 0 : b;
        b = b > 1 ? 1 : b;
        g = g < 0 ? 0 : g;
        g = g > 1 ? 1 : g;
        image[i].Red = r * 255.0;
        image[i].Green = g * 255.0;
        image[i].Blue = b * 255.0;
    }
}
