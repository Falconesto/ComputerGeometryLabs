//
// Created by Nikolay Rulev on 17.03.2020.
//
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cerrno>
#include <utility>
#include "Image.h"


void Image::read(const string &input) {
    FILE *f = fopen(input.c_str(), "rb");
    if (f == nullptr) {
        throw std::runtime_error("Can't open input file");
    }
    int type, h, w, depth;
    if(fscanf(f, "P%i%i%i%i\n", &type, &w, &h, &depth) != 4) {
        throw std::runtime_error("Wrong image format");
    }
    if(type != 6 || depth != 255) {
        throw std::runtime_error("Wrong image format");
    }
    width_ = w;
    height_ = h;
    buffer[0].resize(h);
    buffer[1].resize(h);
    buffer[2].resize(h);
    vector <unsigned  char> tmp(w * 3);
    for(int i = 0; i < h; i++) {
        buffer[0][i].resize(w);
        buffer[1][i].resize(w);
        buffer[2][i].resize(w);
        if(fread(tmp.data(), 1, w * 3, f) != w  * 3) {
            //cerr << strerror(errno) << endl;
            throw std::runtime_error("Can't read input file");
        }
        for(int j = 0; j < width_ * 3; j += 3) {
            buffer[0][i][j / 3] = (double)tmp[j];
            buffer[1][i][j / 3] = (double)tmp[j + 1];
            buffer[2][i][j / 3] = (double)tmp[j + 2];
        }
    }
    if(fclose(f)) {
        throw std::runtime_error("Can't close input file");
    }
}
void Image::read(const string &input1, const string &input2, const string &input3) {
    auto read_file = [&] (const string &file_name, int num) {
        FILE *f = fopen(file_name.c_str(), "rb");
        if (f == nullptr) {
            throw std::runtime_error("Can't open one of input files");
        }
        int type, h, w, depth;
        if(fscanf(f, "P%i%i%i%i\n", &type, &w, &h, &depth) != 4) {
            throw std::runtime_error("Wrong image format");
        }
        if(type != 5 || depth != 255) {
            throw std::runtime_error("Wrong image format");
        }
        width_ = w;
        height_ = h;
        buffer[num].resize(h);
        vector <unsigned  char> tmp(w);
        for(int i = 0; i < h; i++) {
            buffer[num][i].resize(w);
            if(fread(tmp.data(), 1, w, f) != w) {
                throw std::runtime_error("Can't read input file");
            }
            for(int j = 0; j < width_; j++) {
                buffer[num][i][j] = (double)tmp[j];
            }
        }
        if(fclose(f)) {
            throw std::runtime_error("Can't close input file");
        }
    };
    read_file(input1, 0);
    read_file(input2, 1);
    read_file(input3, 2);
}


void Image::write(const string &output) {
    FILE *f = fopen(output.c_str(), "wb");
    if (f == nullptr) {
        throw std::runtime_error("Can't open output file");
    }
    if(fprintf(f, "P6\n%i %i\n255\n", width_, height_) < 0) {
        throw std::runtime_error("Can't write in output file");
    }
    vector <unsigned  char> tmp(width_ * 3);
    for(int i = 0; i < height_; i++) {
        for(int j = 0; j < width_ * 3; j += 3) {
            for(int k = 0; k < 3; k++) {
                if(buffer[k][i][j / 3] > 255) {
                    buffer[k][i][j / 3] = 255;
                } else if(buffer[k][i][j / 3] < 0) {
                    buffer[k][i][j / 3] = 0;
                }
            }
            tmp[j] = (unsigned char)buffer[0][i][j / 3];
            tmp[j + 1] = (unsigned char)buffer[1][i][j / 3];
            tmp[j + 2] = (unsigned char)buffer[2][i][j / 3];
        }
        if( fwrite(tmp.data(), 1, width_ * 3, f) != width_ * 3) {
            throw std::runtime_error("Can't write in output file");
        }
    }
    if(fclose(f)) {
        throw std::runtime_error("Can't close output file");
    }
}

void Image::write(const string &output1, const string &output2, const string &output3) {
    auto write_file = [&] (const string &file_name, int num) {
        FILE *f = fopen(file_name.c_str(), "wb");
        if (f == nullptr) {
            throw std::runtime_error("Can't open output file");
        }
        if(fprintf(f, "P5\n%i %i\n255\n", width_, height_) < 0) {
            throw std::runtime_error("Can't write in output file");
        }
        vector <unsigned  char> tmp(width_);
        for(int i = 0; i < height_; i++) {
            for(int j = 0; j < width_; j++) {
                if(buffer[num][i][j] > 255) {
                    buffer[num][i][j] = 255;
                } else if(buffer[num][i][j] < 0) {
                    buffer[num][i][j] = 0;
                }
                tmp[j] = (char)buffer[num][i][j];
            }
            if( fwrite(tmp.data(), 1, width_, f) != width_) {
                throw std::runtime_error("Can't write in output file");
            }
        }
        if(fclose(f)) {
            throw std::runtime_error("Can't close output file");
        }
    };
    write_file(output1, 0);
    write_file(output2, 1);
    write_file(output3, 2);
}

void Image::convert(const string &from, const string &to) {
    //RGB / HSL / HSV / YCbCr.601 / YCbCr.709 / YCoCg / CMY
    if(from == "HSL") {
        hsl_rgb();
    } else if(from == "HSV") {
        hsv_rgb();
    } else if(from == "YCbCr.601") {
        ycbcr601_rgb();
    } else if(from == "YCbCr.709") {
        ycbcr709_rgb();
    } else if(from == "YCoCg") {
        ycocg_rgb();
    } else if(from == "CMY") {
        cmy_rgb();
    }

    // =========

    if(to == "HSL") {
        rgb_hsl();
    } else if(to == "HSV") {
        rgb_hsv();
    } else if(to == "YCbCr.601") {
        rgb_ycbcr601();
    } else if(to == "YCbCr.709") {
        rgb_ycbcr709();
    } else if(to == "YCoCg") {
        rgb_ycocg();
    } else if(to == "CMY") {
        rgb_cmy();
    }

}

void Image::hsl_rgb() {
    auto hue2rgb = [] (double p, double q, double t) {
        if(t < 0) t += 1;
        if(t > 1) t -= 1;
        if(t < 1.0/6) return p + (q - p) * 6 * t;
        if(t < 1.0/2) return q;
        if(t < 2.0/3) return p + (q - p) * (2.0/3 - t) * 6;
        return p;
    };
    for(int H = 0; H < height_; H++) {
        for(int W = 0; W < width_; W++) {
            double h = buffer[0][H][W] / 255.0;
            double s = buffer[1][H][W] / 255.0;
            double l = buffer[2][H][W] / 255.0;
            if(s == 0) {
                buffer[0][H][W] = buffer[1][H][W] = buffer[2][H][W] = l * 255.0;
                continue;
            }
            double q = (l < 0.5) ? l * (1 + s) : l + s - l * s;
            double p = 2 * l - q;
            buffer[0][H][W] = hue2rgb(p, q, h + 1.0 / 3) * 255;
            buffer[1][H][W] = hue2rgb(p, q, h) * 255;
            buffer[2][H][W] = hue2rgb(p, q, h - 1.0 / 3) * 255;
        }
    }
}

void Image::hsv_rgb() {
    for(int H = 0; H < height_; H++) {
        for(int W = 0; W < width_; W++) {
            double h = buffer[0][H][W] / 255;
            double s = buffer[1][H][W] / 255;
            double v = buffer[2][H][W] / 255;

            double r = 0, g = 0, b = 0, f, p, q, t;
            int i = floor(h * 6);
            f = h * 6 - i;
            p = v * (1 - s);
            q = v * (1 - f * s);
            t = v * (1 - (1 - f) * s);
            if(i % 6 == 0) {
                r = v, g = t, b = p;
            } else if(i % 6 == 1) {
                r = q, g = v, b = p;
            } else if(i % 6 == 2) {
                r = p, g = v, b = t;
            } else if(i % 6 == 3) {
                r = p, g = q, b = v;
            } else if(i % 6 == 4) {
                r = t, g = p, b = v;
            } else if(i % 6 == 5) {
                r = v, g = p, b = q;
            }
            buffer[0][H][W] = r * 255;
            buffer[1][H][W] = g * 255;
            buffer[2][H][W] = b * 255;
        }
    }

}

void Image::ycbcr601_rgb() {
    double Kr = 0.299;
    double Kb = 0.114;

    for(int H = 0; H < height_; H++) {
        for (int W = 0; W < width_; W++) {
            double Y = buffer[0][H][W] / 255;
            double Cb = buffer[1][H][W] / 255 * 2 - 1;
            double Cr = buffer[2][H][W] / 255 * 2 - 1;

            double R = Y + (1 - Kr) * Cr;
            double G = Y - Kb * (1 - Kb) * Cb / (1 - Kb - Kr) - Kr * (1 - Kr) * Cr / (1 - Kb - Kr);
            double B = Y + (1 - Kb) * Cb;

            buffer[0][H][W] = R * 255;
            buffer[1][H][W] = G * 255;
            buffer[2][H][W] = B * 255;
        }
    }
}

void Image::ycbcr709_rgb() {
    double Kr = 0.2126;
    double Kb = 0.0722;

    for(int H = 0; H < height_; H++) {
        for (int W = 0; W < width_; W++) {
            double Y = buffer[0][H][W] / 255;
            double Cb = buffer[1][H][W] / 255 * 2 - 1;
            double Cr = buffer[2][H][W] / 255 * 2 - 1;

            double R = Y + (1 - Kr) * Cr;
            double G = Y - Kb * (1 - Kb) * Cb / (1 - Kb - Kr) - Kr * (1 - Kr) * Cr / (1 - Kb - Kr);
            double B = Y + (1 - Kb) * Cb;

            buffer[0][H][W] = R * 255;
            buffer[1][H][W] = G * 255;
            buffer[2][H][W] = B * 255;
        }
    }
}

void Image::ycocg_rgb() {
    for(int H = 0; H < height_; H++) {
        for(int W = 0; W < width_; W++) {
            double Y = buffer[0][H][W] / 255;
            double Co = buffer[1][H][W] / 255 - 0.5;
            double Cg = buffer[2][H][W] / 255  - 0.5;

            double R = Y + Co - Cg;
            double G = Y + Cg;
            double B = Y - Co - Cg;

            buffer[0][H][W] = R * 255;
            buffer[1][H][W] = G * 255;
            buffer[2][H][W] = B * 255;
        }
    }
}

void Image::cmy_rgb() {
    for(int H = 0; H < height_; H++) {
        for(int W = 0; W < width_; W++) {
            buffer[0][H][W] = (1.0 - buffer[0][H][W] / 255.0) * 255.0;
            buffer[1][H][W] = (1.0 - buffer[1][H][W] / 255.0) * 255.0;
            buffer[2][H][W] = (1.0 - buffer[2][H][W] / 255.0) * 255.0;
        }
    }
}

void Image::rgb_hsl() {
    for(int H = 0; H < height_; H++) {
        for(int W = 0; W < width_; W++) {
            double r = buffer[0][H][W] / 255;
            double g = buffer[1][H][W] / 255;
            double b = buffer[2][H][W] / 255;
            double mx = max(r, max(g, b));
            double mn = min(r, min(g, b));
            double h, s, l = (mn + mx) / 2;
            if(mx == mn) {
                h = s = 0;
            } else {
                double d = mx - mn;
                s = (l > 0.5) ? d / (2 - mx - mn) : d / (mx + mn);
                if(mx == r) {
                    h = (g - b) / d + ((g < b) ? 6 : 0);
                } else if(mx == g) {
                    h = (b - r) / d + 2;
                } else {
                    h = (r - g) / d + 4;
                }
                h /= 6;
            }
            buffer[0][H][W] = h * 255;
            buffer[1][H][W] = s * 255;
            buffer[2][H][W] = l * 255;
        }
    }
}

void Image::rgb_hsv() {
    for(int H = 0; H < height_; H++) {
        for(int W = 0; W < width_; W++) {
            double r = buffer[0][H][W] / 255;
            double g = buffer[1][H][W] / 255;
            double b = buffer[2][H][W] / 255;
            double mx = max(r, max(g, b));
            double mn = min(r, min(g, b));

            double h, s, v = mx;

            double d = mx - mn;
            s = (mx == 0) ? 0 : d / mx;
            if (mx == mn) {
                h = 0;
            } else {
                if(mx == r) {
                    h = (g - b) / d + (g < b ? 6 : 0);
                } else if(mx == g) {
                    h = (b - r) / d + 2;
                } else {
                    h = (r - g) / d + 4;
                }
                h /= 6;
            }
            buffer[0][H][W] = h * 255;
            buffer[1][H][W] = s * 255;
            buffer[2][H][W] = v * 255;

        }
    }
}

void Image::rgb_ycbcr601() {
    double Kr = 0.299;
    double Kb = 0.114;

    for (int H = 0; H < height_; H++) {
        for (int W = 0; W < width_; W++) {
            double R = buffer[0][H][W] / 255;
            double G = buffer[1][H][W] / 255;
            double B = buffer[2][H][W] / 255;

            double Y = Kr * R + (1 - Kb - Kr) * G + Kb * B;
            double Cb = (B - Y) / (1 - Kb) + 1;
            double Cr = (R - Y) / (1 - Kb) + 1;

            buffer[0][H][W] = Y * 255;
            buffer[1][H][W] = Cb * 255 / 2;
            buffer[2][H][W] = Cr * 255 / 2;
        }
    }
}

void Image::rgb_ycbcr709() {
    double Kr = 0.2126;
    double Kb = 0.0722;

    for (int H = 0; H < height_; H++) {
        for (int W = 0; W < width_; W++) {
            double R = buffer[0][H][W] / 255;
            double G = buffer[1][H][W] / 255;
            double B = buffer[2][H][W] / 255;

            double Y = Kr * R + (1 - Kb - Kr) * G + Kb * B;
            double Cb = (B - Y) / (1 - Kb) + 1;
            double Cr = (R - Y) / (1 - Kb) + 1;

            buffer[0][H][W] = Y * 255;
            buffer[1][H][W] = Cb * 255 / 2;
            buffer[2][H][W] = Cr * 255 / 2;
        }
    }
}

void Image::rgb_ycocg() {
    for(int H = 0; H < height_; H++) {
        for(int W = 0; W < width_; W++) {
            double R = buffer[0][H][W] / 255.0;
            double G = buffer[1][H][W] / 255.0;
            double B = buffer[2][H][W] / 255.0;

            double Y = 1.0/4.0 * R + 1.0/2.0 * G + 1.0/4.0 * B;
            double Co = 1.0/2.0 * R - 1.0/2.0 * B + 1.0/2.0;
            double Cg = -1.0/4.0 * R + 1.0/2.0 * G - 1.0/4.0 * B + 1.0/2.0;

            buffer[0][H][W] = Y * 255;
            buffer[1][H][W] = Co * 255;
            buffer[2][H][W] = Cg * 255;
        }
    }
}

void Image::rgb_cmy() {
    for(int H = 0; H < height_; H++) {
        for(int W = 0; W < width_; W++) {
            buffer[0][H][W] = (1.0 - buffer[0][H][W] / 255.0) * 255.0;
            buffer[1][H][W] = (1.0 - buffer[1][H][W] / 255.0) * 255.0;
            buffer[2][H][W] = (1.0 - buffer[2][H][W] / 255.0) * 255.0;
        }
    }
}
