//
// Created by Александр on 07.06.2020.
//

#include "Image.h"
#include <cmath>
#include <algorithm>


Image::Image(FILE *file) {
    if (fscanf(file, "P%d", &this->format) == 0) {
        std::cerr << "Please check the format of the file" << std::endl;
        fclose(file);
        exit(1);
    }

    fscanf(file, "%d %d", &this->width, &this->height);
    fscanf(file, "%d\n", &this->colors);

    if ((this->height < 1) || (this->width < 1)) {
        std::cerr << "Please check the height or width in the FrameHeader" << std::endl;
        fclose(file);
        exit(1);
    }
    if (this->colors != 255) {
        std::cerr << "Please check the colours in the FrameHeader" << std::endl;
        fclose(file);
        exit(1);
    }

    data = (unsigned char *) malloc(sizeof(unsigned char) * this->height * this->width);
    if (data == NULL) {
        std::cerr << "Can`t free my mind for data array" << std::endl;
        fclose(file);
        exit(1);
    }
    int troubleLess = fread(data, sizeof(char), (this->height * this->width), file);
    if (troubleLess < (this->height * this->width)) {
        std::cerr << "Please check the number of the pixels: IT IS NOT ENOUGH" << std::endl;
        fclose(file);
        free(data);
        exit(1);
    }
    int troubleMore = fread(data, sizeof(char), 1, file);
    if (troubleMore > 0) {
        std::cerr << "Please check the number of the pixels: OH GOD, IT IS TOO MUCH" << std::endl;
        fclose(file);
        free(data);
        exit(1);
    }
}

void Image::printInFile(FILE *outputFile) {
    fprintf(outputFile, "P%d\n", this->format);
    fprintf(outputFile, "%d %d\n", this->width, this->height);
    fprintf(outputFile, "%d\n", this->colors);
    fwrite(this->data, sizeof(char), (this->height * this->width), outputFile);
    free(data);
}

void Image::dither(algorithm algo, unsigned char bitNumber) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            get(i, j) = gammaFromPictureToArray(get(i, j));
        }
    }
    switch (algo) {
        case No:
            None(bitNumber);
            break;
        case ordered:
            Ordered(bitNumber);
            break;
        case random:
            Random(bitNumber);
            break;
        case floyd_stein:
            Floyd(bitNumber);
            break;
        case jarvis:
            Jarvis(bitNumber);
            break;
        case sierra:
            Sierra(bitNumber);
            break;
        case atkinson:
            Atkinson(bitNumber);
            break;
        case halftone:
            Halftone(bitNumber);
            break;
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            get(i, j) = gammaFromArrayToPicture(get(i, j));
        }
    }
}

int Image::new_bit_colour(int x, int bits) {
    x >>= (8 - bits);
    x <<= (8 - bits);
    int tmp_bits = bits;
    int tmp_x = x;
    while (bits <= 8) {
        x |= tmp_x >> bits;
        bits += tmp_bits;
    }
    return x;
}

unsigned char &Image::get(int h, int w) {
    return data[width * h + w];
}

double &Image::get_error(vector<double> &errors, int h, int w) {
    return errors[h * width + w];
}

void Image::None(unsigned char bitRate) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double value = get(i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);

            double new_bit_color = new_bit_colour(round(value * 255), int(bitRate));
            get(i, j) = new_bit_color;
        }
    }
}

void Image::Ordered(unsigned char bitRate) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pixelColor = get(i, j) / 255.0;
            double value = pixelColor + (orderedTable[i % 8][j % 8] - 0.5) / bitRate;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = new_bit_colour(round(value * 255), int(bitRate));
            get(i, j) = new_bit_color;
        }
    }
}

void Image::Random(unsigned char bitRate) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pixelColor = (get(i, j) / 255.0);
            double noise = (((double) (rand() % 256) / 256) - 0.5);
            double value = pixelColor + noise;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = new_bit_colour(round(value * 255), int(bitRate));
            get(i, j) = new_bit_color;
        }
    }
}

void Image::Floyd(unsigned char bitRate) {
    vector<double> errors(height * width, 0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pixelColor = get(i, j) / 255.0;
            double value = pixelColor + get_error(errors, i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = (new_bit_colour(round(value * 255), int(bitRate)));
            double error = value * 255 - new_bit_color;
            get(i, j) = new_bit_color;
            if (j + 1 < width) {
                get_error(errors, i, j + 1) += error * 7.0 / 16.0;
            }
            if (i + 1 < height && j + 1 < width) {
                get_error(errors, i + 1, j + 1) += error * 1.0 / 16.0;
            }
            if (i + 1 < height) {
                get_error(errors, i + 1, j) += error * 5.0 / 16.0;
            }
            if (i + 1 < height && j - 1 >= 0) {
                get_error(errors, i + 1, j - 1) += error * 3.0 / 16.0;
            }
        }
    }
}

    void Image::Jarvis(unsigned char bitRate) {
        vector<double> errors(height * width, 0);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                double pixelColor = get(i, j) / 255.0;
                double value = pixelColor + (get_error(errors, i, j) / 255.0);
                value = min(max(value, 0.0), 1.0);
                double new_bit_color = new_bit_colour(round(value * 255), int(bitRate));
                double error = value * 255 - new_bit_color;
                get(i, j) = new_bit_color;
                for (int alfa = 0; alfa < 3; alfa++) {
                    for (int beta = 0; beta < 5; beta++) {
                        if (i + alfa >= height || j + (beta - 2) >= width || j + (beta - 2) < 0) {
                            continue;
                        }
                        if (alfa == 0 && beta <= 2) {
                            continue;
                        }
                        get_error(errors, i + alfa, j + (beta - 2)) += error * JarvisTable[alfa][beta];
                    }
                }
            }
        }
    }

void Image::Sierra(unsigned char bitRate) {
    vector<double> errors(height * width, 0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pixelColor = get(i, j) / 255.0;
            double value = pixelColor + get_error(errors, i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = new_bit_colour(round(value * 255), int(bitRate));
            double error = value * 255 - (new_bit_color);
            get(i, j) = new_bit_color;
            for (int alfa = 0; alfa < 3; alfa++) {
                for (int beta = 0; beta < 5; beta++) {
                    if (i + alfa >= height || j + (beta - 2) >= width || j + (beta - 2) < 0)
                        continue;
                    if (alfa == 0 && beta <= 2)
                        continue;
                    get_error(errors, i + alfa, j + (beta - 2)) += error * SIERRATable[alfa][beta];
                }
            }
        }
    }
}

void Image::Atkinson(unsigned char bitRate) {
    vector<double> errors(height * width, 0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pixelColor = get(i, j) / 255.0;
            double value = pixelColor + get_error(errors, i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = new_bit_colour(round(value * 255), int(bitRate));
            double error = pixelColor * 255 + get_error(errors, i, j) - new_bit_color;
            get(i, j) = new_bit_color;

            for (int alfa = 0; alfa < 3; alfa++) {
                for (int beta = 0; beta < 4; beta++) {
                    if (i + alfa >= height || j + (beta - 1) >= width || j + (beta - 1) < 0)
                        continue;
                    if (alfa == 0 && beta <= 1)
                        continue;
                    get_error(errors, i + alfa, j + (beta - 1)) += error * ATKINSONTable[alfa][beta] / 8.0;
                }
            }
        }
    }
}

void Image::Halftone(unsigned char bitRate) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pixelColor = round(get(i, j)) / 255.0;
            double value = pixelColor + (halftoneTable[i % 4][j % 4] - 0.5);
            value = min(max(value, 0.0), 1.0);
            double new_bit_color = new_bit_colour(round(value * 255), int(bitRate));
            get(i, j) = round(round(new_bit_color));
        }
    }
}

void Image::setGamma(double g) {
    gamma = g;
}

void Image::createGradient() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            get(i, j) = gammaFromArrayToPicture(round((double) j / (width-1) * 255));
        }
    }
}

double Image::gammaFromPictureToArray(double value) {
    double tmpPixel = value / (double) 255;
    if (gamma == 0) {
        if (tmpPixel <= 0.04045) {
            tmpPixel /= 12.95;
        } else {
            tmpPixel = pow((tmpPixel + 0.055) / 1.055, 2.4);
        }
    } else {
        tmpPixel = pow(tmpPixel,  gamma);
    }
    return round(tmpPixel * 255);
}

double Image::gammaFromArrayToPicture(double value) {
    double tmpPixel = value / (double) 255;
    if (gamma == 0) {
        if (tmpPixel <= 0.0031308) {
            tmpPixel *= 12.92;
        } else {
            tmpPixel = pow(tmpPixel * 1.055, 0.4167) - 0.055;
        }
    } else {
        tmpPixel = pow(tmpPixel, 1/gamma);
    }
    return round(tmpPixel * 255);
}