#include <iostream>
#include "Picture.h"


int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "rb");
    if (file == nullptr) {
        std::cerr << "Can`t open input file" << std::endl;
        return -1;
    }

    Conversion NewConversion;
    double difference;
    double multiple;

    if (argc < 4) {
        std::cerr << "Incorrect amount of arguments\n";
        return 1;
    }

    std::string input_file;
    std::string output_file;
    input_file = argv[1];
    output_file = argv[2];

    auto conversion = atoi(argv[3]);

    if ((conversion != 0) && (conversion != 2) && (conversion != 1) && (conversion != 3)
        && (conversion != 4) && (conversion != 5)) {
        std::cerr << "Incorrect type of conversion , conversion must be integer from 0 to 5 \n"
                     "Check arguments of 3";
        return 1;
    }

    NewConversion = static_cast<Conversion>(atoi(argv[3]));

    if ((NewConversion == RGB_with) || (NewConversion == YCBCR_with)) {
        if (argc != 6) {
            std::cerr << "Not enough paramets for conversion\n "
                         "For 0 and 1 conversion we must have difference and multiple";
            return 1;
        }
        difference = atoi(argv[4]);
        multiple = atof(argv[5]);
    }

    Picture picture;

    if ((NewConversion == YCBCR_with) || (NewConversion == YCBCR_AUTO) || (NewConversion == YCBCR_classic)) {
        picture.whichType(YCbCr601);
    }

    picture.Open_File(input_file);

    if ((NewConversion == RGB_with) || (NewConversion == YCBCR_with)) {
        picture.changeBrightness(difference, multiple);
    } else {
        picture.Brightness((NewConversion == RGB_AUTO) || (NewConversion == YCBCR_AUTO));
    }
    picture.Save_File(output_file);
    return 0;
}