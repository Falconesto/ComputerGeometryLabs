#include <fstream>
#include <iostream>
#include "Picture.h"


void Picture::input(std::string filename) {
    std::ifstream input;
    input.open(filename, std::ios_base::in | std::ios_base::binary);
    input >> format;

    if (format == "P5") {
        color = false;
    } else {
        if (format == "P6") {
            color = true;
        } else {
            std::cerr << "wrong format" << std::endl;
        }
    }
    input >> width >> height;
    input >> colour;
    if (colour != 255) {
        std::cerr << "Unacceptable format" << std::endl;
    }

    char partPixel;
    char *tmp_pix = &partPixel;
    data = new Pix[width * height];
    input.read(tmp_pix, 1);

    for (int i = 0; i < width * height; i++) {
        if (color) {
            input.read(tmp_pix, 1);
            data[i].r = *tmp_pix;
            input.read(tmp_pix, 1);
            data[i].g = *tmp_pix;
            input.read(tmp_pix, 1);
            data[i].b = *tmp_pix;
        } else {
            input.read(tmp_pix, 1);
            data[i].r = *tmp_pix;
            data[i].g = *tmp_pix;
            data[i].b = *tmp_pix;
        }
    }
    input.close();
}

void Picture::output(std::string filename) {
    std::ofstream output;
    output.open(filename, std::ios_base::out | std::ios_base::binary);

    if (color) {
        output << "P6" << '\n';
    } else {
        output << "P5" << '\n';
    }
    output << width << ' ' << height << '\n' << colour << '\n';

    char *image_char = (char *) data;
    if (color) {
        output.write(image_char, width * height * 3);
    } else {
        for (int i = 0; i < width * height; i++) {
            output.write(&image_char[i * 3], 1);
        }
    }
    output.close();
}

void Picture::from_RGB_to_YCbCr601() {
    double kr = 0.299, kb = 0.114;
    for (int i = 0; i < width * height; i++) {
        double r = data[i].r / 255.0;
        double g = data[i].g / 255.0;
        double b = data[i].b / 255.0;
        double y = kr * r + (1 - kr - kb) * g + kb * b;
        double pb = 0.5 * (b - y) / (1.0 - kb);
        double pr = 0.5 * (r - y) / (1.0 - kr);
        data[i].r = y * 255.0;
        data[i].g = (pb + 0.5) * 255.0;
        data[i].b = (pr + 0.5) * 255.0;
    }
}

void Picture::from_YCbCr601_to_RGB() {
    double kr = 0.299, kb = 0.114;
    for (int i = 0; i < width * height; i++) {
        double y = data[i].r / 255.0;
        double pb = data[i].g / 255.0 - 0.5;
        double pr = data[i].b / 255.0 - 0.5;
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
        data[i].r = r * 255.0;
        data[i].g = g * 255.0;
        data[i].b = b * 255.0;
    }
}

void Picture::second_step(double diff, double mult) {
    if (color) {
        if (type == RGB) {
            for (int i = 0; i < width * height; i++) {
                int result = ((double) data[i].r - diff) * mult;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                data[i].r = result;
                result = ((double) data[i].g - diff) * mult;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                data[i].g = result;
                result = ((double) data[i].b - diff) * mult;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                data[i].b = result;
            }
        } else {
            for (int i = 0; i < width * height; i++) {
                int result = ((double) data[i].r - diff) * mult;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                data[i].r = result;
            }
        }
    } else {
        if (type == RGB) {
            for (int i = 0; i < width * height; i++) {
                int result = ((double) data[i].r - diff) * mult;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                data[i].r = result;
                data[i].g = result;
                data[i].b = result;
            }
        } else {
            for (int i = 0; i < width * height; i++) {
                int result = ((double) data[i].r - diff) * mult;
                if (result > 255) result = 255;
                if (result < 0) result = 0;
                data[i].r = result;
            }
        }
    }
}

void Picture::first_step(bool type) {
    std::vector<int> get;
    get = get_min_and_max(type);
    double diff = (get[1]);
    double mult = 255.0 / (double) (get[0] - get[1]);
    std::cout << diff << " " << mult;
    second_step(diff, mult);
}

std::vector<int> Picture::get_min_and_max(bool if_r_or_y) {
    int pixels[256] = {};
    int max_pix = 0;
    int min_pix = 0;
    for (int i = 0; i < width * height; i++) {
        if ((color) && (type != YCbCr601)) {
            pixels[data[i].r]++;
            pixels[data[i].g]++;
            pixels[data[i].b]++;
        } else {
            pixels[data[i].r]++;
        }
    }
    if (if_r_or_y) {
        int to_delete_ = 0;
        int delete_1 = 0;
        int Dposition = 0;
        int Bposition = 255;
        if ((color) && (type != YCbCr601)) {
            to_delete_ = (double) (width * height) * 3 * 0.0039;
        } else {
            to_delete_ = (double) (width * height) * 0.0039;
        }
        int x = to_delete_;
        int delete_2 = 0;
        while (delete_1 < x) {
            while (pixels[Dposition] == 0) {
                Dposition++;
            }
            pixels[Dposition]--;
            delete_1++;
        }
        while (delete_2 < x) {
            while (pixels[Bposition] == 0) {
                Bposition--;
            }
            pixels[Bposition]--;
            delete_2++;
        }
    }
    for (int i = 0; i < 256; i++) {
        if (pixels[i] > 0) {
            min_pix = i;
            break;
        }
    }
    for (int i = 255; i >= 0; i--) {
        if (pixels[i] > 0) {
            max_pix = i;
            break;
        }
    }
    return {max_pix, min_pix};
}

