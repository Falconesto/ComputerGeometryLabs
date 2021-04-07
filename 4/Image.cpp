//
// Created by Александр on 11.06.2020.
//


#include "Image.h"
#include <string.h>
#include <algorithm>

void Image::readOneFile(myInput fileData) {
    FILE *f = fopen(fileData.inputFile, "rb");
    if (!f) {
        cerr << "myInput file didn't open" << endl;
        exit(1);
    }
    data = new fileHeader[1];
    data[0].file = f;
    if (fscanf(f, "P%d%d%d%d\n", &this->data[0].format, &this->data[0].width, &this->data[0].height,
               &data[0].colors) != 4) {
        cerr << "Wrong amount data in file" << endl;
        fclose(f);
        exit(1);
    }
    if (data[0].format != 6) {
        cerr << "Wrong format" << endl;
        fclose(f);
        exit(1);
    }
    data[0].size = data[0].width * data[0].height;
    unsigned char *tmpArray;
    tmpArray = new unsigned char[data[0].size * 3];
    pixelData = new Pixel[data[0].size];
    fread(tmpArray, sizeof(unsigned char), data[0].size * 3, f);
    for (int i = 0, j = 0; i < data[0].size * 3; i += 3, j++) {
        pixelData[j] = {(double) tmpArray[i], (double) tmpArray[i + 1], (double) tmpArray[i + 2]};
    }
    delete[] tmpArray;
    fclose(f);
}

void Image::readThreeFiles(myInput fileData) {
    FILE *inputFile1, *inputFile2, *inputFile3;
    string filename1 = fileData.inputFile;
    string filename2 = filename1;
    string filename3 = filename1;
    filename1.insert(filename1.size() - 4, "_1");
    filename2.insert(filename2.size() - 4, "_2");
    filename3.insert(filename3.size() - 4, "_3");
    char *name1 = new char[filename1.size() + 1];
    char *name2 = new char[filename2.size() + 1];
    char *name3 = new char[filename3.size() + 1];
    strcpy(name1, filename1.c_str());
    strcpy(name2, filename2.c_str());
    strcpy(name3, filename3.c_str());
    inputFile1 = fopen(name1, "rb");
    if (!inputFile1) {
        cerr << "First file didn't open" << endl;
        exit(1);
    }
    inputFile2 = fopen(name2, "rb");
    if (!inputFile2) {
        cerr << "Second file didn't open" << endl;
        fclose(inputFile1);
        exit(1);
    }
    inputFile3 = fopen(name3, "rb");
    if (!inputFile3) {
        cerr << "Third file didn't open" << endl;
        fclose(inputFile1);
        fclose(inputFile2);
        exit(1);
    }
    data = new fileHeader[3];
    fscanf(inputFile1, "P%d%d%d%d\n", &this->data[0].format, &this->data[0].width, &this->data[0].height,
           &data[0].colors);
    fscanf(inputFile2, "P%d%d%d%d\n", &this->data[1].format, &this->data[1].width, &this->data[1].height,
           &data[1].colors);
    fscanf(inputFile3, "P%d%d%d%d\n", &this->data[2].format, &this->data[2].width, &this->data[2].height,
           &data[2].colors);
    if (data[0].colors != 255) {
        cerr << "Wrong colors in first file" << endl;
        fclose(inputFile1);
        fclose(inputFile2);
        fclose(inputFile3);
        exit(1);
    }
    if (data[1].colors != 255) {
        cerr << "Wrong colors in second file" << endl;
        fclose(inputFile1);
        fclose(inputFile2);
        fclose(inputFile3);
        exit(1);
    }
    if (data[2].colors != 255) {
        cerr << "Wrong colors in third file" << endl;
        fclose(inputFile1);
        fclose(inputFile2);
        fclose(inputFile3);
        exit(1);
    }
    if (data[0].format != 5) {
        cerr << "Wrong format of first file" << endl;
        fclose(inputFile1);
        fclose(inputFile2);
        fclose(inputFile3);
        exit(1);
    }
    if (data[1].format != 5) {
        cerr << "Wrong format of second file" << endl;
        fclose(inputFile1);
        fclose(inputFile2);
        fclose(inputFile3);
        exit(1);
    }
    if (data[2].format != 5) {
        cerr << "Wrong format of third file" << endl;
        fclose(inputFile1);
        fclose(inputFile2);
        fclose(inputFile3);
        exit(1);
    }
    int width = data[0].width;
    if (data[1].width != width || data[2].width != width) {
        cerr << "Wrong widths" << endl;
        fclose(inputFile1);
        fclose(inputFile2);
        fclose(inputFile3);
        exit(1);
    }
    int height = data[0].height;
    if (data[1].height != height || data[2].height != height) {
        cerr << "Wrong heights" << endl;
        fclose(inputFile1);
        fclose(inputFile2);
        fclose(inputFile3);
        exit(1);
    }
    data[0].size = width * height;
    data[1].size = width * height;
    data[2].size = width * height;
    int len = data[0].size;
    unsigned char *tmpArray1 = new unsigned char[len];
    fread(tmpArray1, sizeof(unsigned char), len, inputFile1);
    unsigned char *tmpArray2 = new unsigned char[len];
    fread(tmpArray2, sizeof(unsigned char), len, inputFile2);
    unsigned char *tmpArray3 = new unsigned char[len];
    fread(tmpArray3, sizeof(unsigned char), len, inputFile3);
    pixelData = new Pixel[len];
    for (int i = 0; i < len; i++) {
        pixelData[i] = {(double) tmpArray1[i], (double) tmpArray2[i], (double) tmpArray3[i]};
    }
    delete[] tmpArray1;
    delete[] tmpArray2;
    delete[] tmpArray3;
    fclose(inputFile1);
    fclose(inputFile2);
    fclose(inputFile3);
}

void Image::writeOneFile(myInput fileData) {
    FILE *outputFile = fopen(fileData.outputFile, "wb");
    if (!outputFile) {
        cerr << "Output file didn't open" << endl;
        exit(1);
    }
    fprintf(outputFile, "P%d\n%d %d\n%d\n", 6, data[0].width, data[0].height, data[0].colors);
    int len = data[0].size * 3;
    unsigned char *buffer = new unsigned char[len];
    for (int i = 0, j = 0; i < len; i += 3, j++) {
        buffer[i] = (unsigned char) pixelData[j].red;
        buffer[i + 1] = (unsigned char) pixelData[j].green;
        buffer[i + 2] = (unsigned char) pixelData[j].blue;
    }
    fwrite(buffer, sizeof(unsigned char), len, outputFile);
    delete[] buffer;
    fclose(outputFile);
}

void Image::writeThreeFiles(myInput fileData) {
    FILE *f1, *f2, *f3;
    string filename1 = fileData.outputFile;
    string filename2 = filename1;
    string filename3 = filename1;
    filename1.insert(filename1.size() - 4, "_1");
    filename2.insert(filename2.size() - 4, "_2");
    filename3.insert(filename3.size() - 4, "_3");
    int n = filename1.size();
    char *name1 = new char[filename1.size() + 1];
    char *name2 = new char[filename2.size() + 1];
    char *name3 = new char[filename3.size() + 1];
    strcpy(name1, filename1.c_str());
    strcpy(name2, filename2.c_str());
    strcpy(name3, filename3.c_str());
    f1 = fopen(name1, "wb");
    if (!f1) {
        cerr << "First output file didn't open" << endl;
        exit(1);
    }
    f2 = fopen(name2, "wb");
    if (!f2) {
        cerr << "Second output file didn't open" << endl;
        fclose(f1);
        exit(1);
    }
    f3 = fopen(name3, "wb");
    if (!f3) {
        cerr << "Third output file didn't open" << endl;
        fclose(f1);
        fclose(f2);
        exit(1);
    }
    fprintf(f1, "P%i\n%d %d\n%d\n", 5, data[0].width, data[0].height, data[0].colors);
    fprintf(f2, "P%i\n%d %d\n%d\n", 5, data[0].width, data[0].height, data[0].colors);
    fprintf(f3, "P%i\n%d %d\n%d\n", 5, data[0].width, data[0].height, data[0].colors);
    unsigned char *buffer1 = new unsigned char[data[0].size];
    unsigned char *buffer2 = new unsigned char[data[0].size];
    unsigned char *buffer3 = new unsigned char[data[0].size];
    for (int i = 0; i < data[0].size; i++) {
        buffer1[i] = (unsigned char) pixelData[i].red;
        buffer2[i] = (unsigned char) pixelData[i].green;
        buffer3[i] = (unsigned char) pixelData[i].blue;
    }
    fwrite(buffer1, sizeof(unsigned char), data[0].size, f1);
    fwrite(buffer2, sizeof(unsigned char), data[0].size, f2);
    fwrite(buffer3, sizeof(unsigned char), data[0].size, f3);
    delete[] buffer1;
    delete[] buffer2;
    delete[] buffer3;
    fclose(f1);
    fclose(f2);
    fclose(f3);
}

void Image::action(myInput newInput) {
    switch (newInput.from) {
        case 0:
            break;
        case 1:
            HSLtoRGB();
            break;
        case 2:
            HSVtoRGB();
            break;
        case 3:
            YCbCr601toRGB();
            break;
        case 4:
            YCbCr709toRGB();
            break;
        case 5:
            YCoCgtoRGB();
            break;
        case 6:
            CMYtoRGB();
            break;
    }
    switch (newInput.to) {
        case 0:
            break;
        case 1:
            RGBtoHSL();
            break;
        case 2:
            RGBtoHSV();
            break;
        case 3:
            RGBtoYCbCr601();
            break;
        case 4:
            RGBtoYCbCr709();
            break;
        case 5:
            RGBtoYCoCg();
            break;
        case 6:
            RGBtoCMY();
            break;
    }
}

void Image::RGBtoCMY() {
    for (int i = 0; i < data->size; i++) {
        pixelData[i].red = (1 - pixelData[i].red / 255.0) * 255.0;
        pixelData[i].green = (1 - pixelData[i].green / 255.0) * 255.0;
        pixelData[i].blue = (1 - pixelData[i].blue / 255.0) * 255.0;
    }
}

void Image::CMYtoRGB() {
    for (int i = 0; i < data->size; i++) {
        pixelData[i].red = (1 - pixelData[i].red / 255.0) * 255.0;
        pixelData[i].green = (1 - pixelData[i].green / 255.0) * 255.0;
        pixelData[i].blue = (1 - pixelData[i].blue / 255.0) * 255.0;
    }
}

void Image::RGBtoHSL() {
    for (int i = 0; i < data->size; i++) {
        double r = pixelData[i].red / 255.0;
        double g = pixelData[i].green / 255.0;
        double b = pixelData[i].blue / 255.0;
        double Max = max(r, max(g, b));
        double Min = min(r, min(g, b));
        double V = Max;
        double C = Max - Min;
        double L = V - C / 2.0;
        double H;
        if (C == 0)
            H = 0;
        else {
            if (V == r)
                H = (60.0) * ((g - b) / C);
            else if (V == g)
                H = (60.0) * (2 + (b - r) / C);
            else if (V == b)
                H = (60.0) * (4 + (r - g) / C);
            else
                H = 0;
        }
        double S = ((L == 0) || (L == 1)) ? 0 : ((V - L) / min(L, 1 - L));
        pixelData[i].blue = L * 255.0;
        pixelData[i].green = S * 255.0;
        pixelData[i].red = (H / 360.0) * 255.0;
    }
}

void Image::HSLtoRGB() {
    for (int i = 0; i < data->size; i++) {
        double h = (pixelData[i].red / 255.0) * 360.0;
        double s = pixelData[i].green / 255.0;
        double l = pixelData[i].blue / 255.0;
        double h1 = h / 60.0;
        double c = (1 - abs(2 * l - 1)) * s;
        double x = c * (1 - abs(fmod(h1, 2) - 1));
        double m = l - c / 2.0;
        m *= 255.0;
        if ((h1 >= 0) && (h1 <= 1)) {
            pixelData[i].red = c * 255.0 + m;
            pixelData[i].green = x * 255.0 + m;
            pixelData[i].blue = 0 + m;
        }
        if ((h1 > 1) && (h1 <= 2)) {
            pixelData[i].red = x * 255.0 + m;
            pixelData[i].green = c * 255.0 + m;
            pixelData[i].blue = 0 + m;
        }
        if ((h1 > 2) && (h1 <= 3)) {
            pixelData[i].red = 0 + m;
            pixelData[i].green = c * 255.0 + m;
            pixelData[i].blue = x * 255.0 + m;
        }
        if ((h1 > 3) && (h1 <= 4)) {
            pixelData[i].red = 0 + m;
            pixelData[i].green = x * 255.0 + m;
            pixelData[i].blue = c * 255.0 + m;
        }
        if ((h1 > 4) && (h1 <= 5)) {
            pixelData[i].red = x * 255.0 + m;
            pixelData[i].green = 0 + m;
            pixelData[i].blue = c * 255.0 + m;
        }
        if ((h1 > 5) && (h1 <= 6)) {
            pixelData[i].red = c * 255.0 + m;
            pixelData[i].green = 0 + m;
            pixelData[i].blue = x * 255.0 + m;
        }
    }
}

void Image::RGBtoHSV() {
    for (int i = 0; i < data->size; i++) {
        double r = pixelData[i].red / 255.0;
        double g = pixelData[i].green / 255.0;
        double b = pixelData[i].blue / 255.0;
        double MAX = max(r, max(g, b));
        double MIN = min(r, min(g, b));
        double h, s, v;
        if (MAX == MIN) {
            h = 0;
        } else if (MAX == r && g >= b) {
            h = 60.0 * (g - b) / (MAX - MIN);
        } else if (MAX == r && g < b) {
            h = 60.0 * (g - b) / (MAX - MIN) + 360.0;
        } else if (MAX == g) {
            h = 60.0 * (b - r) / (MAX - MIN) + 120.0;
        } else if (MAX == b) {
            h = 60.0 * (r - g) / (MAX - MIN) + 240.0;
        }
        if (MAX == 0) {
            s = 0;
        } else {
            s = 1 - MIN / MAX;
        }
        v = MAX;
        pixelData[i].red = h / 360.0 * 255.0;
        pixelData[i].green = s * 255.0;
        pixelData[i].blue = v * 255.0;
    }
}

void Image::HSVtoRGB() {
    for (int i = 0; i < data->size; i++) {
        double h = pixelData[i].red / 255.0 * 360.0;
        double s = pixelData[i].green / 255.0;
        double V = pixelData[i].blue / 255.0;
        double r, g, b;
        double Vmin = (1 - s) * V;
        double Hi = abs(fmod(h / 60.0, 6));
        pixelData[i].red = r * 255.0;
        pixelData[i].green = g * 255.0;
        pixelData[i].blue = b * 255.0;
        double a = (V - Vmin) * (fmod(h, 60)) / 60.0;
        double Vinc = Vmin + a;
        double Vdec = V - a;
        switch ((int) Hi) {
            case 0:
                r = V;
                g = Vinc;
                b = Vmin;
                break;
            case 1:
                r = Vdec;
                g = V;
                b = Vmin;
                break;
            case 2:
                r = Vmin;
                g = V;
                b = Vinc;
                break;
            case 3:
                r = Vmin;
                g = Vdec;
                b = V;
                break;
            case 4:
                r = Vinc;
                g = Vmin;
                b = V;
                break;
            case 5:
                r = V;
                g = Vmin;
                b = Vdec;
                break;
        }
        pixelData[i].red = r * 255.0;
        pixelData[i].green = g * 255.0;
        pixelData[i].blue = b * 255.0;
    }
}

void Image::RGBtoYCbCr601() {
    double kr = 0.299, kb = 0.114;
    for (int i = 0; i < data->size; i++) {
        double r = pixelData[i].red / 255.0;
        double g = pixelData[i].green / 255.0;
        double b = pixelData[i].blue / 255.0;
        double y = kr * r + (1 - kr - kb) * g + kb * b;
        double pb = 0.5 * (b - y) / (1.0 - kb);
        double pr = 0.5 * (r - y) / (1.0 - kr);
        pixelData[i].red = y * 255.0;
        pixelData[i].green = (pb + 0.5) * 255.0;
        pixelData[i].blue = (pr + 0.5) * 255.0;
    }
}

void Image::RGBtoYCbCr709() {
    double kr = 0.0722, kb = 0.7152;
    for (int i = 0; i < data->size; i++) {
        double r = pixelData[i].red / 255.0;
        double g = pixelData[i].green / 255.0;
        double b = pixelData[i].blue / 255.0;
        double y = kr * r + (1 - kr - kb) * g + kb * b;
        double pb = 0.5 * (b - y) / (1.0 - kb);
        double pr = 0.5 * (r - y) / (1.0 - kr);
        pixelData[i].red = y * 255.0;
        pixelData[i].green = (pb + 0.5) * 255.0;
        pixelData[i].blue = (pr + 0.5) * 255.0;
    }
}

void Image::YCbCr601toRGB() {
    double kr = 0.299, kb = 0.114;
    for (int i = 0; i < data->size; i++) {
        double y = pixelData[i].red / 255.0;
        double pb = pixelData[i].green / 255.0 - 0.5;
        double pr = pixelData[i].blue / 255.0 - 0.5;
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
        pixelData[i].red = r * 255.0;
        pixelData[i].green = g * 255.0;
        pixelData[i].blue = b * 255.0;
    }
}

void Image::YCbCr709toRGB() {
    double kr = 0.0722, kg = 0.2126, kb = 0.7152;
    for (int i = 0; i < data->size; i++) {
        double y = pixelData[i].red / 255.0;
        double pb = pixelData[i].green / 255.0 - 0.5;
        double pr = pixelData[i].blue / 255.0 - 0.5;
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
        pixelData[i].red = r * 255.0;
        pixelData[i].green = g * 255.0;
        pixelData[i].blue = b * 255.0;
    }
}

void Image::RGBtoYCoCg() {
    for (int i = 0; i < data->size; i++) {
        double r = pixelData[i].red / 255.0;
        double g = pixelData[i].green / 255.0;
        double b = pixelData[i].blue / 255.0;
        double y = r / 4.0 + g / 2.0 + b / 4.0;
        double cg = -r / 4.0 + g / 2.0 - b / 4.0;
        double co = r / 2.0 - b / 2.0;
        pixelData[i].red = y * 255.0;
        pixelData[i].green = (co + 0.5) * 255.0;
        pixelData[i].blue = (cg + 0.5) * 255.0;

    }
}

void Image::YCoCgtoRGB() {
    for (int i = 0; i < data->size; i++) {
        double y = pixelData[i].red / 255.0;
        double co = pixelData[i].green / 255.0 - 0.5;
        double cg = pixelData[i].blue / 255.0 - 0.5;
        double r = y + co - cg;
        double g = y + cg;
        double b = y - co - cg;
        r = r < 0 ? 0 : r;
        r = r > 1 ? 1 : r;
        b = b < 0 ? 0 : b;
        b = b > 1 ? 1 : b;
        g = g < 0 ? 0 : g;
        g = g > 1 ? 1 : g;
        pixelData[i].red = r * 255.0;
        pixelData[i].green = g * 255.0;
        pixelData[i].blue = b * 255.0;
    }
}

Image::~Image() {
    delete[] pixelData;
    delete[] data;
}