//
// Created by Александр on 23.04.2020.
//


#include "Image.h"


Image::Image(FILE *file) {
    if (fscanf(file, "P%d", &this->format) == 0) {
        std::cerr << "Please check the format of the file" << std::endl;
        fclose(file);
        exit(1);
    }

    fscanf(file, "%d %d", &this->width, &this->height);
    fscanf(file, "%d\n", &this->colours);

    if ((this->height < 1) || (this->width < 1)) {
        std::cerr << "Please check the height or width in the FrameHeader" << std::endl;
        fclose(file);
        exit(1);
    }
    if (this->colours != 255) {
        std::cerr << "Please check the colours in the FrameHeader" << std::endl;
        fclose(file);
        exit(1);
    }

    switch (this->format) {
        case 5:
            formatVariable = 1;
            break;
        case 6:
            formatVariable = 3;
            break;
        default:
            std::cerr << "Please check the format of the file -> (P-something)" << std::endl;
            fclose(file);
            exit(1);
    }

    data = (unsigned char *) malloc(sizeof(unsigned char) * formatVariable * this->height * this->width);
    if (data == NULL) {
        std::cerr << "Can`t free my mind for data array" << std::endl;
        fclose(file);
        exit(1);
    }
    int troubleLess = fread(data, sizeof(char), formatVariable * (this->height * this->width), file);
    if (troubleLess < formatVariable * (this->height * this->width)) {
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

void Image::putPoint(int x, int y, double brightness, unsigned int color) {
    if ((x >= 0) && (y >= 0) && (x <= this->width) && (y <= this->height) && brightness != 0) {
        double background = gammaFromPictureToArray(this->data[y * this->width + x]);
        double newColor = gammaFromPictureToArray(color);
        double newValue = background * (1 - brightness) + newColor * brightness;
        this->data[y * this->width + x] = gammaFromArrayToPicture(newValue);
    }
}

void Image::printInFile(FILE *outputFile) {
    fprintf(outputFile, "P%d\n", this->format);
    fprintf(outputFile, "%d %d\n", this->width, this->height);
    fprintf(outputFile, "%d\n", this->colours);
    fwrite(this->data, sizeof(char), (formatVariable * this->height * this->width), outputFile);
}

void Image::drawLine(Line *line) {
    this->setGamma(line->gamma);
    if (line->start.x > line->end.x) {
        std::swap(line->start, line->end);
    }

    if ((line->thickness <= 1)) {
        if (line->start.x == line->end.x) {
            drawStraightThinLineY(line, line->brightness * line->thickness);
        } else {
            if (line->start.y == line->end.y) {
                drawStraightThinLineX(line, line->brightness * line->thickness);
            } else {
                drawWuLine(line->start, line->end, line->thickness, line->brightness);
            }
        }
    } else {
        if (line->start.x == line->end.x) {
            line->start.y = line->start.y - 0.5;
            line->end.y = line->end.y - 0.5;
            drawStraightLineY(line, line->brightness);
        } else {
            if (line->start.y == line->end.y) {
                line->start.x = line->start.x - 0.5;
                line->end.x = line->end.x - 0.5;
                drawStraightLineX(line, line->brightness);
            } else {
                line->start.x = line->start.x - 0.5;
                line->end.x = line->end.x - 0.5;
                line->start.y = line->start.y - 0.5;
                line->end.y = line->end.y - 0.5;
                double x0 = line->start.x;
                double y0 = line->start.y;
                double x1 = line->end.x;
                double y1 = line->end.y;
                double lineWidth = line->thickness;

                double tmpAngle = atan((x1 - x0) / (y1 - y0));
                double dx = (lineWidth / 2) * cos(tmpAngle) * tmpAngle / abs(tmpAngle);
                double dy = (lineWidth / 2) * sin(tmpAngle) * tmpAngle / abs(tmpAngle);
                std::vector<Point> rectPoints = getDots(line, dx, dy);
                if (abs(x1 - x0) == abs(y1 - y0)) {
                    draw45Line(rectPoints, line->brightness);
                } else {
                    drawWuLine(rectPoints[0], rectPoints[1], 1, line->brightness);
                    drawWuLine(rectPoints[1], rectPoints[3], 1, line->brightness);
                    drawWuLine(rectPoints[3], rectPoints[2], 1, line->brightness);
                    drawWuLine(rectPoints[2], rectPoints[0], 1, line->brightness);

                    fillLine(rectPoints, line->brightness);
                }
            }
        }
    }
}

std::vector<Point> Image::getDots(Line *line, double dx, double dy) {
    Point start = line->start;
    Point end = line->end;
    Point one;
    one.x = start.x + dx;
    one.y = start.y - dy;
    Point two;
    two.x = start.x - dx;
    two.y = start.y + dy;
    Point three;
    three.x = end.x + dx;
    three.y = end.y - dy;
    Point four;
    four.x = end.x - dx;
    four.y = end.y + dy;
    std::vector<Point> answer = {one, two, three, four};
    return answer;
}

void Image::draw45Line(std::vector<Point> points, int brightness) {
    for (int i = round(std::min(std::min(points[0].y, points[1].y), std::min(points[2].y, points[3].y)));
         i <= round(std::max(std::max(points[0].y, points[1].y), std::max(points[2].y, points[3].y))); i++) {
        for (int j = round(std::min(std::min(points[0].x, points[1].x), std::min(points[2].x, points[3].x)));
             j <= round(std::max(std::max(points[0].x, points[1].x), std::max(points[2].x, points[3].x))); j++) {
            if (isInside45(points, j, i)) {
                int count = 0;
                if (isInside(points, j, i)) {
                    count++;
                }
                for (int k = 1; k < 5; k++) {
                    if (isInside(points, j + (k * 0.125), i + (k * 0.125))) {
                        count++;
                    }
                    if (isInside(points, j - (k * 0.125), i + (k * 0.125))) {
                        count++;
                    }
                    if (isInside(points, j + (k * 0.125), i - (k * 0.125))) {
                        count++;
                    }
                    if (isInside(points, j - (k * 0.125), i - (k * 0.125))) {
                        count++;
                    }
                }
                putPoint(j, i, pow(((double) count / (double) 17), 1 / 1.4), brightness);
            }
        }
    }
}

double Image::fPart(double x) {
    return abs(x - round(x));
}

double Image::lengthToCenterOfAPixel(double x, double y) {
    return std::sqrt(pow(fPart(x), 2.0) + pow(fPart(y), 2.0));
}

double Image::lengthToCenterOfAnotherPixel(double x, double y, double x0, double y0) {
    return std::min(1.0, sqrt(pow(x0 - x, 2) + pow(y0 - y, 2)));
}

void Image::fillLine(std::vector<Point> points, int brightness) {
    for (int i = round(std::min(std::min(points[0].y, points[1].y), std::min(points[2].y, points[3].y)));
         i < round(std::max(std::max(points[0].y, points[1].y), std::max(points[2].y, points[3].y))); i++) {
        for (int j = round(std::min(std::min(points[0].x, points[1].x), std::min(points[2].x, points[3].x)));
             j < round(std::max(std::max(points[0].x, points[1].x), std::max(points[2].x, points[3].x))); j++) {
            if (isInside(points, j, i)) {
                putPoint(j, i, 1, brightness);
            }
        }
    }
}

bool Image::isInside(std::vector<Point> points, int checkX, int checkY) { // 0-правее, 1- левее
    return ((isInsideXtoTheLeft(points[2], points[3], checkX, checkY)) &&
            (isInsideXtoTheRight(points[0], points[1], checkX, checkY)) &&
            (isInsideYtoTheUp(points[1], points[3], checkX, checkY)) &&
            (isInsideYtoTheDown(points[0], points[2], checkX, checkY)));
}

bool Image::isInside45(std::vector<Point> points, int checkX, int checkY) { // 0-правее, 1- левее
    return ((isInsideXtoTheLeft(points[2], points[3], checkX - 0.5, checkY + 0.5)) ||
            (isInsideXtoTheRight(points[0], points[1], checkX + 0.5, checkY - 0.5)) ||
            (isInsideYtoTheUp(points[1], points[3], checkX - 0.5, checkY - 0.5)) ||
            (isInsideYtoTheDown(points[0], points[2], checkX + 0.5, checkY + 0.5)) ||
            (isInsideXtoTheLeft(points[2], points[3], checkX - 0.5, checkY - 0.5)) ||
            (isInsideXtoTheRight(points[0], points[1], checkX + 0.5, checkY + 0.5)) ||
            (isInsideYtoTheUp(points[1], points[3], checkX + 0.5, checkY - 0.5)) ||
            (isInsideYtoTheDown(points[0], points[2], checkX - 0.5, checkY + 0.5)));
}

bool Image::isInsideXtoTheLeft(Point first, Point second, int checkX, int checkY) { // 0-правее, 1- левее
    return ((((checkY - first.y) * (second.x - first.x) / (second.y - first.y)) + first.x) >= (checkX) + 0.1) ||
           ((((checkY - first.y) * (second.x - first.x) / (second.y - first.y)) + first.x) >= (checkX) - 0.1);
}

bool Image::isInsideXtoTheRight(Point first, Point second, int checkX, int checkY) { // 0-левее, 1- правее
    return ((((checkY - first.y) * (second.x - first.x) / (second.y - first.y)) + first.x) <= (checkX) + 0.1) ||
           ((((checkY - first.y) * (second.x - first.x) / (second.y - first.y)) + first.x) <= (checkX) - 0.1);
}

bool Image::isInsideYtoTheUp(Point first, Point second, int checkX, int checkY) { // 0-ниже, 1- выше
    return ((((second.y - first.y) * (checkX - first.x) / (second.x - first.x)) + first.y) >= (checkY) + 0.1) ||
           ((((second.y - first.y) * (checkX - first.x) / (second.x - first.x)) + first.y) >= (checkY) - 0.1);
}

bool Image::isInsideYtoTheDown(Point first, Point second, int checkX, int checkY) { // 0-выше, 1- ниже
    return ((((second.y - first.y) * (checkX - first.x) / (second.x - first.x)) + first.y) <= (checkY) + 0.1) ||
           ((((second.y - first.y) * (checkX - first.x) / (second.x - first.x)) + first.y) <= (checkY) - 0.1);
}

void Image::drawWuLine(Point start, Point end, double color, int brightness) {
    double x0 = start.x;
    double y0 = start.y;
    double x1 = end.x;
    double y1 = end.y;
    double dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    double dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);


    //Для Х-линии (коэффициент наклона < 1)
    if (dy < dx) {
        if (x1 < x0) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        double angleVariable = dy / dx;
        //Промежуточная переменная для Y
        if (y1 < y0) {
            angleVariable *= -1;
        }
        double yVariable = y0 + angleVariable;
        //Первая точка
        this->putPoint(round(x0), round(y0), (1 - lengthToCenterOfAPixel(x0, y0))*color, brightness);
        for (int x = round(x0 + 1); x < round(x1); x++) {
            this->putPoint(x, round(yVariable), (1 - fPart(yVariable))*color, brightness);
            if (yVariable - round(yVariable) > 0) {
                this->putPoint(x, round(yVariable) + 1, fPart(yVariable)*color, brightness);
            } else {
                this->putPoint(x, round(yVariable) - 1, fPart(yVariable)*color, brightness);
            }
            yVariable += angleVariable;
        }
        this->putPoint(round(x1), round(y1), (1 - lengthToCenterOfAPixel(x1, y1))*color, brightness);

    } else {
        //Первая точка должна иметь меньшую координату Y
        if (y1 < y0) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        //Относительное изменение координаты X
        double angleVariable = dx / dy;
        //Промежуточная переменная для X
        if (x1 < x0) {
            angleVariable *= -1;
        }
        double xVariable = x0 + angleVariable;
        //Первая точка
        this->putPoint(round(x0), round(y0), (1 - lengthToCenterOfAPixel(x0, y0))*color, brightness);
        for (int y = round(y0 + 1); y < round(y1); y++) {
            this->putPoint(round(xVariable), y, (1 - fPart(xVariable))*color, brightness);
            if (xVariable - round(xVariable) > 0) {
                this->putPoint(round(xVariable) + 1, y, fPart(xVariable)*color, brightness);
            } else {
                this->putPoint(round(xVariable) - 1, y, fPart(xVariable)*color, brightness);
            }
            xVariable += angleVariable;
        }
        this->putPoint(round(x1), round(y1), (1 - lengthToCenterOfAPixel(x1, y1))*color, brightness);
    }
}

void Image::drawStraightLineX(Line *line, double brightness) {
    std::vector<Point> points = getDots(line, 0, line->thickness / 2);
    int dyUp = points[0].y > round(points[0].y) ? 1 : -1;
    int dyDown = points[1].y > round(points[1].y) ? 1 : -1;
    double x0 = points[0].x + 1;
    double y0 = points[0].y;
    while (round(x0) < round(points[2].x)) {
        putPoint(round(x0), round(y0), (0.5 - dyUp * lengthToCenterOfAPixel(round(x0), y0)), brightness);
        x0++;
    }
    x0 = points[1].x + 1;
    y0 = points[1].y;
    while (round(x0) < round(points[3].x)) {
        putPoint(round(x0), round(y0), (0.5 + dyDown * lengthToCenterOfAPixel(round(x0), y0)), brightness);
        x0++;
    }

    int dxLeft = points[0].x > round(points[0].x) ? 1 : -1;
    int dxRight = points[1].x > round(points[1].x) ? 1 : -1;
    x0 = points[0].x;
    y0 = points[0].y + 1;
    while (round(y0) < round(points[1].y)) {
        putPoint(round(x0), round(y0), (0.5 - dxLeft * lengthToCenterOfAPixel(x0, round(y0))), brightness);
        y0++;
    }
    x0 = points[2].x;
    y0 = points[2].y + 1;
    while (round(y0) < round(points[3].y)) {
        putPoint(round(x0), round(y0), (0.5 + dxRight * lengthToCenterOfAPixel(x0, round(y0))), brightness);
        y0++;
    }
    for (int i = 0; i < 4; i++) {
        int dSthX = (i / 2) * 2 - 1;
        int dSthY = (i % 2) * 2 - 1;
        putPoint(round(points[i].x), round(points[i].y), abs(points[i].x - (round(points[i].x) - 0.5 * dSthX)) *
                                                         abs(points[i].y - (round(points[i].y) - 0.5 * dSthY)),
                 brightness);
    }
    for (int x0 = round(points[0].x) + 1; x0 <= round(points[2].x) - 1; x0++) {
        for (int y0 = round(points[0].y) + 1; y0 <= round(points[1].y) - 1; y0++) {
            putPoint(x0, y0, 1, brightness);
        }
    }
}

void Image::drawStraightLineY(Line *line, double brightness) {
    if (line->start.y > line->end.y) {
        std::swap(line->start, line->end);
    }
    std::vector<Point> points = getDots(line, line->thickness / 2, 0);
    std::swap(points[0], points[1]);
    std::swap(points[3], points[1]);
    std::swap(points[3], points[2]);
    int dyUp = points[0].y > round(points[0].y) ? 1 : -1;
    int dyDown = points[1].y > round(points[1].y) ? 1 : -1;
    double x0 = points[0].x + 1;
    double y0 = points[0].y;
    while (round(x0) < round(points[2].x)) {
        putPoint(round(x0), round(y0), (0.5 - dyUp * lengthToCenterOfAPixel(round(x0), y0)), brightness);
        x0++;
    }
    x0 = points[1].x + 1;
    y0 = points[1].y;
    while (round(x0) < round(points[3].x)) {
        putPoint(round(x0), round(y0), (0.5 + dyDown * lengthToCenterOfAPixel(round(x0), y0)), brightness);
        x0++;
    }

    int dxLeft = points[0].x > round(points[0].x) ? 1 : -1;
    int dxRight = points[2].x > round(points[2].x) ? 1 : -1;
    x0 = points[0].x;
    y0 = points[0].y + 1;
    while (round(y0) < round(points[1].y)) {
        putPoint(round(x0), round(y0), (0.5 - dxLeft * lengthToCenterOfAPixel(x0, round(y0))), brightness);
        y0++;
    }
    x0 = points[2].x;
    y0 = points[2].y + 1;
    while (round(y0) < round(points[3].y)) {
        putPoint(round(x0), round(y0), (0.5 + dxRight * lengthToCenterOfAPixel(x0, round(y0))), brightness);
        y0++;
    }
    for (int i = 0; i < 4; i++) {
        int dSthX = (i / 2) * 2 - 1;
        int dSthY = (i % 2) * 2 - 1;
        putPoint(round(points[i].x), round(points[i].y), abs(points[i].x - (round(points[i].x) - 0.5 * dSthX)) *
                                                         abs(points[i].y - (round(points[i].y) - 0.5 * dSthY)),
                 brightness);
    }
    for (int x0 = round(points[0].x) + 1; x0 <= round(points[2].x) - 1; x0++) {
        for (int y0 = round(points[0].y) + 1; y0 <= round(points[1].y) - 1; y0++) {
            putPoint(x0, y0, 1, brightness);
        }
    }


//    std::vector<Point> points = getDots(line, line->thickness / 2, 0);
//    for (int i = 0; i < 2; i++) {
//        double x0 = points[i * 2 + 1].x + 1;
//        double y0 = points[i * 2 + 1].y;
//        while (round(x0) < round(points[i * 2].x)) {
//            putPoint(round(x0), round(y0), (1 - lengthToCenterOfAPixel(round(x0), y0)), brightness);
//            x0++;
//        }
//    }
//    for (int i = 0; i < 2; i++) {
//        double x0 = points[i].x;
//        double y0 = points[i].y + 1;
//        while (round(y0) < round(points[i + 2].y)) {
//            putPoint(round(x0), round(y0), (1 - lengthToCenterOfAPixel(x0, round(y0))), brightness);
//            y0++;
//        }
//    }
//    for (int i = 0; i < 4; i++) {
//        putPoint(round(points[i].x), round(points[i].y), (1 - lengthToCenterOfAPixel(points[i].x, points[i].y)),
//                 brightness);
//    }
//    for (int x0 = round(points[1].x) + 1; x0 <= round(points[0].x) - 1; x0++) {
//        for (int y0 = round(points[1].y) + 1; y0 <= round(points[3].y) - 1; y0++) {
//            putPoint(x0, y0, 1, brightness);
//        }
//    }
}

void Image::drawStraightThinLineX(Line *line, double brightness) {
    if (line->start.x > line->end.x) {
        std::swap(line->start, line->end);
    }
    double x0 = round(line->start.x) + 1;
    int pixelShiftY = line->start.y > round(line->start.y) ? 1 : -1;
    putPoint(round(line->start.x), round(line->start.y),
             (1.0 - lengthToCenterOfAPixel(line->start.x, line->start.y)), brightness);
    putPoint(round(line->end.x), round(line->end.y),
             (1.0 - lengthToCenterOfAPixel(line->end.x, line->end.y)), brightness);
    putPoint(round(line->start.x), round(line->start.y) + pixelShiftY,
             (1.0 - lengthToCenterOfAnotherPixel(line->start.x, line->start.y, round(line->start.x),
                                                 round(line->start.y + pixelShiftY))), brightness);
    putPoint(round(line->end.x), round(line->end.y) + pixelShiftY,
             (1.0 - lengthToCenterOfAnotherPixel(line->end.x, line->end.y, round(line->end.x),
                                                 round(line->end.y) + pixelShiftY)), brightness);
    while (x0 < round(line->end.x)) {
        putPoint(x0, round(line->start.y) + pixelShiftY,
                 (0.5 + (line->thickness / 2) -
                  lengthToCenterOfAnotherPixel(x0, line->start.y, x0, round(line->start.y) + pixelShiftY)),
                 brightness);
        putPoint(x0, round(line->start.y), (1 - lengthToCenterOfAPixel(x0, line->start.y)), brightness);
        x0++;
    }
}

void Image::drawStraightThinLineY(Line *line, double brightness) {
    if (line->start.y > line->end.y) {
        std::swap(line->start, line->end);
    }
    double y0 = round(line->start.x) + 1;
    int pixelShiftX = line->start.x > round(line->start.x) ? 1 : -1;
    putPoint(round(line->start.x), round(line->start.y),
             (1.0 - lengthToCenterOfAPixel(line->start.x, line->start.y)), brightness);
    putPoint(round(line->end.x), round(line->end.y),
             (1.0 - lengthToCenterOfAPixel(line->end.x, line->end.y)), brightness);
    putPoint(round(line->start.x) + pixelShiftX, round(line->start.y),
             (1.0 - lengthToCenterOfAnotherPixel(line->start.x, line->start.y, round(line->start.x) + pixelShiftX,
                                                 round(line->start.y))), brightness);
    putPoint(round(line->end.x) + pixelShiftX, round(line->end.y),
             (1.0 - lengthToCenterOfAnotherPixel(line->end.x, line->end.y, round(line->end.x) + pixelShiftX,
                                                 round(line->end.y))), brightness);
    while (y0 < round(line->end.y)) {
        putPoint(round(line->start.x) + pixelShiftX, y0, std::max((0.5 + (line->thickness / 2) -
                                                                   lengthToCenterOfAnotherPixel(line->start.x, y0,
                                                                                                round(line->start.x) +
                                                                                                pixelShiftX, y0)),
                                                                  0.0), brightness);
        putPoint(round(line->start.x), y0, (1 - lengthToCenterOfAPixel(line->start.x, y0)), brightness);
        y0++;
    }
}

double Image::gammaFromPictureToArray(double value) {
    double tmpPixel = value / (double) 255;
    if (gamma == -1) {
        if (tmpPixel <= 0.04045) {
            tmpPixel /= 12.95;
        } else {
            tmpPixel = pow((tmpPixel + 0.055) / 1.055, 2.4);
        }
    } else {
        tmpPixel = pow(tmpPixel, gamma);
    }
    return round(tmpPixel * 255);
}

double Image::gammaFromArrayToPicture(double value) {
    double tmpPixel = value / (double) 255;
    if (gamma == -1) {
        if (tmpPixel <= 0.0031308) {
            tmpPixel *= 12.92;
        } else {
            tmpPixel = pow(tmpPixel * 1.055, 0.4167) - 0.055;
        }
    } else {
        tmpPixel = pow(tmpPixel, 1 / gamma);
    }
    return round(tmpPixel * 255);
}

void Image::setGamma(double gamma) {
    this->gamma = gamma;
}


