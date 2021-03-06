#include <iostream>
#include "CExpension.h"
#include "CLine.h"
#include "CImage.h"

using namespace std;

void getAnsw(const char *fileName, CImage &output) {
    FILE *new_f = fopen(fileName, "wb");
    if (!new_f) {
        throw CExpension("Output file didn't open", new_f);
    }
    fprintf(new_f, "P%i\n%i %i\n%i\n", 5, output.width, output.height, output.max_val);
    fwrite(output.pix, sizeof(unsigned char), output.size, new_f);
    fclose(new_f);
}

int main(int argc, char *argv[]) {
    try {
        SInput input;
        FILE *f;
        if (argc != 10) {
            throw CExpension("Wrong amount of arguments");
        }
        for (int i = 1; i < argc; i++) {
            switch (i) {
                case 1:
                    input.inputName = argv[i];
                    f = fopen(input.inputName, "rb");
                    if (!f) {
                        throw CExpension("Input File didn't open", f);
                    }
                    break;
                case 2:
                    input.outputName = argv[i];
                    break;
                case 3:
                    input.brightness = atoi(argv[i]);
                    break;
                case 4:
                    input.thickness = atof(argv[i]);
                    break;
                case 5:
                    input.start.x_ = atof(argv[i]);
                    break;
                case 6:
                    input.start.y_ = atof(argv[i]);
                    break;
                case 7:
                    input.end.x_ = atof(argv[i]);
                    break;
                case 8:
                    input.end.y_ = atof(argv[i]);
                    break;
                case 9:
                    input.gamma = atof(argv[i]);
                    break;
            }
        }
        CLine line(input);
        CImage image(f);
//        image.BresenhemLine(line);
        image.WuLine(line);
        getAnsw(input.outputName, image);
        return 0;
    } catch (CExpension &expension) {
        cerr << expension.getError();
        if (expension.getFile()) {
            fclose(expension.getFile());
        }
        exit(1);
    }
}