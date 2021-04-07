#include "CLine.h"
#include "CExpension.h"
CLine::CLine(SInput input) {
    this->brightness = input.brightness;
    this->start_ = input.start;
    this->end_ = input.end;
    this->thickness = input.thickness;
    this->gamma_ = input.gamma;
}

