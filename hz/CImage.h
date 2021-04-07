#ifndef GRAPH_CIMAGE_H
#define GRAPH_CIMAGE_H

#include "CExpension.h"
#include <iostream>
#include "CLine.h"

class CImage {
public:
    CImage(FILE *f);

    ~CImage();

    void drawLine(CLine line_);

    void doSmoothing(bool sw, SPoint p, double br, CLine line);

    friend void getAnsw(const char *fileName, CImage &output);

    void BresenhemLine(CLine line_);

    void WuLine(CLine line_);

private:
    int version;
    int width;
    int height;
    int max_val;
    int size;
    unsigned char *pix;


    void putPoint(bool sw, SPoint p, CLine line);

    void BPoint(SPoint p);

    void WuPoint(bool sw, SPoint p, double brigthness, CLine line);

    int gammaCorrection(int value, CLine line);
};


#endif //GRAPH_CIMAGE_H
