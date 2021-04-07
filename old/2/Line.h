//
// Created by Александр on 23.04.2020.
//

#ifndef INC_2_LINE_H
#define INC_2_LINE_H


struct Point {
    double x;
    double y;
};


class Line {
public:
    Line(int brightness, double thickness, Point start, Point end, double gamma);
    double thickness;
    int brightness;
    Point start;
    Point end;
    double gamma = 2.2;

};


#endif //INC_2_LINE_H
