//
// Created by Nikolay Rulev on 17.03.2020.
//

#include "Args.h"

using namespace std;

Args::Args(int argc, char **argv) {

    auto check_color_space = [&] (string &space) -> bool {
        return (space == "RGB" || space == "HSL" || space == "HSV" || space == "YCbCr.601" || space == "YCbCr.709" || space == "CMY" || space == "YCoCg");
    };

    if(argc < 9) {
        throw invalid_argument("Too small arguments, use --help to watch arguments");
    }
    for(int i = 1; i < 11; i++) {
        string temp_arg = argv[i];
        if(temp_arg == "-f") {
            temp_arg = argv[i + 1];
            if(!check_color_space(temp_arg)) {
                throw invalid_argument("color space should be one of RGB / HSL / HSV / YCbCr.601 / YCbCr.709 / YCoCg / CMY");
            } else {
                from_ = temp_arg;
            }
            i++;
        } else if(temp_arg == "-t") {
            temp_arg = argv[i + 1];
            if(!check_color_space(temp_arg)) {
                throw invalid_argument("color space should be one of RGB / HSL / HSV / YCbCr.601 / YCbCr.709 / YCoCg / CMY");
            } else {
                to_ = temp_arg;
            }
            i++;
        } else if(temp_arg == "-i") {
            int int_temp;
            try {
                int_temp = stoi(argv[i + 1]);
            } catch (const invalid_argument &ia) {
                throw invalid_argument("can't parse argument count on input file");
            }
            if(int_temp != 1 && int_temp != 3) {
                throw invalid_argument("count should be 1 or 3");
            }
            input_count_ = int_temp;
            input_file_ = argv[i + 2];
            i += 2;
        } else if(temp_arg == "-o") {
            int int_temp;
            try {
                int_temp = stoi(argv[i + 1]);
            } catch (const invalid_argument &ia) {
                throw invalid_argument("can't parse argument count on input file");
            }
            if(int_temp != 1 && int_temp != 3) {
                throw invalid_argument("count should be 1 or 3");
            }
            output_count_ = int_temp;
            output_file_ = argv[i + 2];
            i += 2;
        } else {
            throw invalid_argument("unexpected argument");
        }
    }
}

const std::string &Args::input_file() const {
    return input_file_;
}

const std::string &Args::output_file() const {
    return output_file_;
}

const std::string &Args::from() const {
    return from_;
}

const std::string &Args::to() const {
    return to_;
}

int Args::input_count() const {
    return input_count_;
}

int Args::output_count() const {
    return output_count_;
}
