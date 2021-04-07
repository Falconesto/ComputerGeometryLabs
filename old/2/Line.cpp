//
// Created by Александр on 23.04.2020.
//

#include "Line.h"

Line::Line(int brightness, double thickness, Point start, Point end, double gamma) {
    this->brightness = brightness;
    this->start = start;
    this->end = end;
    this->thickness = thickness;
    this->gamma = gamma;
}
