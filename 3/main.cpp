#include <iostream>
#include "image.h"

using namespace std;

int main(int argc, char *argv[]) {
    FILE *inputFile = fopen(argv[1], "rb");
    if (inputFile == nullptr) {
        std::cerr << "Can`t open inputFile" << std::endl;
        return 1;
    }
    if (argc != 7) {
        fclose(inputFile);
        std::cerr << "Wrong number of arguments" << endl;
        return 1;
    }
    int myAlgorithm = atoi(argv[4]);
    if (myAlgorithm > 7 || myAlgorithm < 0) {
        fclose(inputFile);
        std::cerr << "Dithering algorithm must be from 0 to 7" << endl;
        return 1;
    }
    FILE *outputFile = fopen(argv[2], "wb");
    if (outputFile == nullptr) {
        fclose(inputFile);
        std::cerr << "Can`t open outputFile" << std::endl;
        return 1;
    }
    bool boolGradient;
    int myGradient = atoi(argv[3]);
    if (myGradient == 1) {
        boolGradient = true;
    } else {
        if (myGradient == 0) {
            boolGradient = false;
        }else {
            fclose(inputFile);
            std::cerr << "Check gradient value" << endl;
            return 1;
        }
    }

    algorithm algoritm = (algorithm) (myAlgorithm);
    int bitsNumber = atoi(argv[5]);
    Image image(inputFile);
    image.setGamma(atof(argv[6]));
    if (boolGradient) {
        image.createGradient();
    }
    image.dither(algoritm, bitsNumber);
    image.printInFile(outputFile);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
