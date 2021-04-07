#include "CImage.h"
#include <cstdlib>
#include <cmath>

using namespace std;

CImage::CImage(FILE *f) {
    if (fscanf(f, "P%i%i%i%i\n", &this->version, &this->width, &this->height, &max_val) != 4) {
        throw CExpension("Wrong amount data in file", f);
    }
    size = width * height;
    pix = new unsigned char[size];
    fread(pix, sizeof(unsigned char), size, f);
    fclose(f);
}

CImage::~CImage() {
    delete[] this->pix;
}

//-----------------------Брезенхем--------------------------





void CImage::BPoint(SPoint p) {
    pix[(int) p.y_ * width + (int) p.x_] = 255;
}


void CImage::BresenhemLine(CLine line_) {
    BPoint(line_.start_);
    BPoint(line_.end_);
    double dx = line_.end_.x_ - line_.start_.x_;
    double dy = line_.end_.y_ - line_.start_.y_;
    int sx = dx < 0 ? -1 : 1;
    int sy = dy < 0 ? -1 : 1;
    dx = fabs(dx);
    dy = fabs(dy);
    double tan = dy / dx;
    double x = line_.start_.x_;
    double y = line_.start_.y_;
    double error = 0;
    double length = dx;
    double dxordy = dy;
    int pdx = sx;
    int pdy = 0;
    if (dy > dx) {
        length = dy;
        dxordy = dx;
        pdx = 0;
        pdy = sy;
    }
    int t = 0;
    while (t <= length) {
        BPoint({x, y});
        error += dxordy;
        if (2 * error >= length) {
            y += sy;
            x += sx;
            error -= length;
        } else {
            y += pdy;
            x += pdx;

        }
        t++;
    }
}


//-----------------------Ву--------------------------

int CImage::gammaCorrection(int value, CLine line) {
    return round(line.brightness * pow((double) value / line.brightness, (double) 1 / (line.gamma_)));
}

void CImage::WuPoint(bool sw, SPoint p, double brigthness, CLine line) {
    int b = (int) round(
            line.brightness * brigthness + (1 - brigthness) * (double) pix[(int) p.x_ * width + (int) p.y_]/255.0);
    int x = sw ? (int) p.y_ : (int) p.x_;
    int y = sw ? (int) p.x_ : (int) p.y_;
    for (int i = 0; i < line.thickness / 2; i++) {
        pix[y * width + x + i] = gammaCorrection(b, line);
    }
}

void CImage::WuLine(CLine line_) {
    bool swapped = fabs(line_.start_.y_ - line_.end_.y_) > fabs(line_.end_.x_ - line_.start_.x_);
    if (swapped) {
        swap(line_.start_.x_, line_.start_.y_);
        swap(line_.end_.x_, line_.end_.y_);
    }
    if (line_.start_.x_ > line_.end_.x_) {
        swap(line_.start_, line_.end_);
    }
    double dx = line_.end_.x_ - line_.start_.x_;
    double dy = line_.end_.y_ - line_.start_.y_;
    double gradient = dy / dx;
    double y = line_.start_.y_;
    double x = line_.start_.x_;
    while (x <= line_.end_.x_) {
        double brightness = fabs(y - (int) y);
        WuPoint(swapped, {x, y}, 1.0 - brightness, line_);
        WuPoint(swapped, {x, y + 1.0}, brightness, line_);
        y += gradient;
        x++;
    }
}