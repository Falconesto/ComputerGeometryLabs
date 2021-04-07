
#include <iostream>
#include "Image.h"

using namespace std;

int main(int argc, char *argv[]) {
    myInput input;
    if (argc != 11) {
        cerr << "Wrong amount of arguments" << endl;
        exit(1);
    }
    string tmp;
    for (int i = 1; i < argc;) {
        tmp = argv[i];
        if (tmp == "-f") {
            i++;
            tmp = argv[i];
            if (tmp == "RGB") {
                input.from = 0;
                i++;
                continue;
            }
            if (tmp == "HSL") {
                input.from = 1;
                i++;
                continue;
            }
            if (tmp == "HSV") {
                input.from = 2;
                i++;
                continue;
            }
            if (tmp == "YCbCr.601") {
                input.from = 3;
                i++;
                continue;
            }
            if (tmp == "YCbCr.709") {
                input.from = 4;
                i++;
                continue;
            }
            if (tmp == "YCoCg") {
                input.from = 5;
                i++;
                continue;
            }
            if (tmp == "CMY") {
                input.from = 6;
                i++;
                continue;
            } else {
                cerr << "Other color space" << endl;
                exit(1);
            }
        }
        tmp = argv[i];
        if (tmp == "-t") {
            i++;
            tmp = argv[i];
            if (tmp == "RGB") {
                input.to = 0;
                i++;
                continue;
            }
            if (tmp == "HSL") {
                input.to = 1;
                i++;
                continue;
            }
            if (tmp == "HSV") {
                input.to = 2;
                i++;
                continue;
            }
            if (tmp == "YCbCr.601") {
                input.to = 3;
                i++;
                continue;
            }
            if (tmp == "YCbCr.709") {
                input.to = 4;
                i++;
                continue;
            }
            if (tmp == "YCoCg") {
                input.to = 5;
                i++;
                continue;
            }
            if (tmp == "CMY") {
                input.to = 6;
                i++;
                continue;
            } else {
                cerr << "Wrong color space" << endl;
                exit(1);
            }
        }
        tmp = argv[i];
        if (tmp == "-i") {
            i++;
            input.countInput = atoi(argv[i]);
            if (input.countInput != 1 && input.countInput != 3) {
                cerr << "Wrong amount of input files" << endl;
                exit(1);
            }
            i++;
            input.inputFile = argv[i];
            i++;
            continue;
        }
        tmp = argv[i];
        if (tmp == "-o") {
            i++;
            input.countOutput = atoi(argv[i]);
            if (input.countOutput != 1 && input.countOutput != 3) {
                cerr << "Wrong amount of input files" << endl;
                exit(1);
            }
            i++;
            input.outputFile = argv[i];
            i++;
            continue;
        }
    }
    Image image;
    if (input.countInput == 1) {
        image.readOneFile(input);
    } else {
        image.readThreeFiles(input);
    }
    image.action(input);
    if (input.countOutput == 1) {
        image.writeOneFile(input);
    } else {
        image.writeThreeFiles(input);
    }
    return 0;
}