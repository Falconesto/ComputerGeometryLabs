//
// Created by Александр on 23.04.2020.
//

#ifndef INC_2_IMAGE_H
#define INC_2_IMAGE_H
#include <iostream>
#include "Line.h"
#include <vector>
#include "math.h"

class Image {
private:
    int format=0;
    int formatVariable=0;
    int width=0;
    int height=0;
    int colours=0;
    unsigned char *data;
    double gamma;

public:
    explicit Image(FILE *file);

    void putPoint(int x, int y, unsigned int colour);

    void printInFile(FILE *outputFile);

    void drawLine(Line *line);

    double fPart(double x);

    void drawWuLine(Point start, Point end, int brightness);

    void fillLine(std::vector<Point> points, int brightness);

    bool isInsideXtoTheLeft(Point first, Point second, int checkX, int checkY);

    bool isInsideXtoTheRight(Point first, Point second, int checkX, int checkY);

    bool isInsideYtoTheUp(Point first, Point second, int checkX, int checkY);

    bool isInsideYtoTheDown(Point first, Point second, int checkX, int checkY);

    void drawStraightLineX(Line *line, double brightness);

    void drawStraightLineY(Line *line, double brightness);

    void drawStraightThinLineX(Line *line, double brightness);

    void drawStraightThinLineY(Line *line, double brightness);

    double lengthToCenterOfAPixel(double x, double y);

    double lengthToCenterOfAnotherPixel(double x, double y, double x0, double y0);

    std::vector<Point> getDots(Line *line, double dx, double dy);

    void gammaFromPictureToArray(int x, int y);

    void gammaFromArrayToPicture(int x, int y);

    void setGamma(double gamma);
};


#endif //INC_2_IMAGE_H
