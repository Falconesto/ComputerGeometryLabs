#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>
#include "Image.h"
#include "Line.h"

FILE *generateFile() {
    FILE *file = fopen("tmpfile.pgm", "wb");
    int height = 510;
    int width = 510;
    fprintf(file, "P%d\n", 5);
    fprintf(file, "%d %d\n", height, width);
    fprintf(file, "%d\n", 255);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(file, "%c", 0);
        }
    }
    fclose(file);
    return file;
}


int main(int argc, char **argv) {
    FILE *tmp = generateFile();
    int formatVariable;


    if ((argc != 10) && (argc != 9)) {
        std::cerr << "Please check the format of the input data" << std::endl;
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "rb");
    if (inputFile == nullptr) {
        std::cerr << "Can`t open inputFile" << std::endl;
        return 1;
    }
    FILE *outputFile = fopen(argv[2], "wb");
    if (outputFile == nullptr) {
        std::cerr << "Can`t open outputFile" << std::endl;
        return 1;
    }
    Point start;
    Point end;
    start.x=atof(argv[5]);
    start.y=atof(argv[6]);
    end.x=atof(argv[7]);
    end.y=atof(argv[8]);
    double gamma;
    if(argc == 10) {
        gamma=atof(argv[9]);
    }else{
        gamma=2.2;
    }
    Line line(atoi(argv[3]), atof(argv[4]), start, end, gamma);
    Image image(inputFile);
    image.drawLine(&line);
    image.printInFile(outputFile);
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}