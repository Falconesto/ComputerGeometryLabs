#include <iostream>
#include "Picture.h"


int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "rb");
    if (file == nullptr) {
        std::cerr << "Can`t open input file" << std::endl;
        return -1;
    }
    if (argc < 4) {
        std::cerr << "Incorrect amount of arguments\n";
        return 1;
    }

    std::string input_file;
    std::string output_file;
    input_file = argv[1];
    output_file = argv[2];
    action folowing_action;
    double difference;
    double multiple;

    folowing_action = static_cast<action>(atoi(argv[3]));

    if ((folowing_action != 0) && (folowing_action != 2) && (folowing_action != 1) && (folowing_action != 3)
        && (folowing_action != 4) && (folowing_action != 5)) {
        std::cerr << "wrong action";
        return 1;
    }

    if ((folowing_action == RGB_with) || (folowing_action == YCBCR_with)) {
        if (argc != 6) {
            std::cerr << "wrong count of arguments for this action";
            return 1;
        }
        difference = atoi(argv[4]);
        multiple = atof(argv[5]);
    }

    Picture picture;
    picture.input(input_file);
    if ((folowing_action == YCBCR_with) || (folowing_action == YCBCR_AUTO) || (folowing_action == YCBCR_classic)) {
        picture.type = YCbCr601;
        picture.from_RGB_to_YCbCr601();
        if (folowing_action == YCBCR_with) {
            picture.second_step(difference, multiple);
        } else {
            picture.first_step(folowing_action == YCBCR_AUTO);
        }
        picture.from_YCbCr601_to_RGB();
    } else {
        if (folowing_action == RGB_with) {
            picture.second_step(difference, multiple);
        } else {
            picture.first_step(folowing_action == RGB_AUTO);
        }
    }
    picture.output(output_file);
    return 0;
}