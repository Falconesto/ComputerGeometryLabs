#include "picture.h"
#include "matrix.h"
#include <cmath>
#include <algorithm>


Picture::Picture() = default;

void Picture::read(string fileName) {
    ifstream inputFile(fileName, ios::binary);
    if (!(inputFile.is_open())) {
        std::cerr<<"Can't open input file"<<std::endl;
        exit(1);
    }
    char P;
    inputFile >> P;
    inputFile >> format;
    if (format!=5) {
        std::cerr<<"Wrong format of image"<<std::endl;
        exit(1);
    }
    inputFile >> width >> height;
    inputFile >> colors;
    if (colors!=255) {
        std::cerr<<"Wrong count of colors"<<std::endl;
        exit(1);
    }
    data.resize(width * height);
    inputFile.read((char *) &data[0], width * height);
    inputFile.close();
}


void Picture::write(const string &fileName) {
    ofstream outputFile(fileName, ios::binary);
    if (!(outputFile.is_open())) {
        std::cerr<<"Can't open output file"<<std::endl;
        exit(1);
    }
    outputFile << "P" << format << '\n';
    outputFile << width << ' ' << height << '\n';
    outputFile << colors << '\n';
    outputFile.write((char *) &data[0], width * height);
    outputFile.close();
}

void Picture::set_gamma(double g) {
    gamma = g;
}

void Picture::gradient() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            get(i, j) = round((double) j / (width - 1) * 255);
        }
    }
}

uchar Picture::correct_color(uchar color) {
    double real_color = color / 255.0;
    double new_color;
    if (gamma == 0) {
        new_color = correct_value_SRGB(real_color);
    } else {
        new_color = correct_value(real_color);
    }
    return new_color * 255;
}

double Picture::correct_value_SRGB(double x) {
    return x <= 0.0031308 ? 12.92 * x : 1.055 * pow(x, 1 / 2.4) - 0.055;
}

double Picture::correct_value(double x) {
    return pow(x, 1/gamma);
}

uchar Picture::undo_color_correction(uchar color) {
    double new_color = color / 255.0;
    double real_color;
    if (gamma == 0) {
        real_color = undo_value_correctionSRGB(new_color);
    } else {
        real_color = undo_value_correction(new_color);
    }
    return real_color * 255;
}

double Picture::undo_value_correctionSRGB(double x) {
    return x <= 0.04045 ? x / 12.92 : pow((x + 0.055) / 1.055, 2.4);
}

double Picture::undo_value_correction(double x) {
    return pow(x, gamma);
}

void Picture::dither(dith_alg alg, uchar bitRate) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            get(i, j) = undo_color_correction(get(i,j));
        }
    }
    switch (alg) {

        case No:
            None(bitRate);
            break;

        case Dith_Ordered:
            Ordered(bitRate);
            break;

        case Dith_Random:
            Random(bitRate);
            break;

        case Dith_F_S:
            F_S(bitRate);
            break;

        case Dith_J_J_N:
            J_J_N(bitRate);
            break;

        case Dith_Sierra:
            Sierra(bitRate);
            break;

        case Dith_Atkinson:
            Atkinson(bitRate);
            break;

        case Dith_Halftone:
            Halftone(bitRate);
            break;

    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            get(i, j) = correct_color(get(i,j));
        }
    }
}

int Picture::new_bit_colour(int x, int bits) {
    x >>= (8 - bits);
    x <<= (8 - bits);
    int tmp_bits=bits;
    int tmp_x=x;
    while(bits <= 8) {
        x |= tmp_x >> bits;
        bits += tmp_bits;
    }
    return x;
}

uchar &Picture::get(int h, int w) {
    return data[width * h + w];
}

double &Picture::get_error(vector<double> &errors, int h, int w) {
    return errors[h * width + w];
}

void Picture::None(uchar bitRate) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double value = get(i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);

            double new_bit_color = new_bit_colour(round(value*255), int(bitRate));
            get(i, j) = new_bit_color;
        }
    }
}

void Picture::Ordered(uchar bitRate) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pic_real_color = get(i, j) / 255.0;
            double value = pic_real_color + (ordered_matrix[i % 8][j % 8] - 0.45) / bitRate;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = new_bit_colour(round(value*255), int(bitRate));
            get(i, j) = new_bit_color;
        }
    }
}

void Picture::Random(uchar bitRate) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pic_real_color = (get(i, j) / 255.0);
            double noise = (((double)(rand() % 256) / 256) - 0.5);
            double value = pic_real_color + noise;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = new_bit_colour(round(value*255), int(bitRate));
            get(i, j) = new_bit_color;
        }
    }
}

void Picture::F_S(uchar bitRate) {
    vector<double> errors(height * width, 0);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pic_real_color = get(i, j) / 255.0;
            double value = pic_real_color + get_error(errors, i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = (new_bit_colour(round(value*255), int(bitRate)));
            double error = value*255 - new_bit_color;
            get(i, j) = new_bit_color;

            if (j + 1 < width) get_error(errors, i, j + 1) += error * 7.0 / 16.0;

            if (i + 1 < height && j + 1 < width) get_error(errors, i + 1, j + 1) += error * 1.0 / 16.0;

            if (i + 1 < height) get_error(errors, i + 1, j) += error * 5.0 / 16.0;

            if (i + 1 < height && j - 1 >= 0) get_error(errors, i + 1, j - 1) += error * 3.0 / 16.0;

        }

    }
}

void Picture::J_J_N(uchar bitRate) {
    vector<double> errors(height * width, 0);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pic_real_color = get(i, j) / 255.0;
            double value = pic_real_color + (get_error(errors, i, j) / 255.0);
            value = min(max(value, 0.0), 1.0);//
            double new_bit_color = new_bit_colour(round(value*255), int(bitRate));
            double error = value*255- new_bit_color;
            get(i, j) = new_bit_color;

            for (int ie = 0; ie < 3; ie++) {
                for (int je = 0; je < 5; je++) {
                    if (i + ie >= height || j + (je - 2) >= width || j + (je - 2) < 0)
                        continue;
                    if (ie == 0 && je <= 2)
                        continue;

                    get_error(errors, i + ie, j + (je - 2)) += error * JJN_matrix[ie][je];
                }
            }
        }
    }
}

void Picture::Sierra(uchar bitRate) {
    vector<double> errors(height * width, 0);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pic_real_color = get(i, j) / 255.0;
            double value = pic_real_color + get_error(errors, i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = new_bit_colour(round(value*255), int(bitRate));
            double error = value * 255 - (new_bit_color);
            get(i, j) = new_bit_color;

            for (int ie = 0; ie < 3; ie++) {
                for (int je = 0; je < 5; je++) {
                    if (i + ie >= height || j + (je - 2) >= width || j + (je - 2) < 0)
                        continue;
                    if (ie == 0 && je <= 2)
                        continue;
                    get_error(errors, i + ie, j + (je - 2)) += error * SIERRA3_matrix[ie][je];
                }
            }
        }
    }
}

void Picture::Atkinson(uchar bitRate) {
    vector<double> errors(height * width, 0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pic_real_color = get(i, j) / 255.0;
            double value = pic_real_color + get_error(errors, i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = new_bit_colour(round(value*255), int(bitRate));
            double error = pic_real_color * 255 + get_error(errors, i, j) - new_bit_color;
            get(i, j) = new_bit_color;

            for (int ie = 0; ie < 3; ie++) {
                for (int je = 0; je < 4; je++) {
                    if (i + ie >= height || j + (je - 1) >= width || j + (je - 1) < 0)
                        continue;
                    if (ie == 0 && je <= 1)
                        continue;
                    get_error(errors, i + ie, j + (je - 1)) += error * ATKINSON_matrix[ie][je] / 8.0;
                }
            }
        }
    }
}

void Picture::Halftone(uchar bitRate) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pic_real_color = round(get(i, j)) / 255.0;
            double value = pic_real_color + (halftone_matrix[i % 4][j % 4] - 0.5) ;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = new_bit_colour(round(value*255), int(bitRate));
            get(i, j) = round(round(new_bit_color));
        }
    }
}
