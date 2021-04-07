#include <iostream>
#include <stdexcept>
#include "Args.h"
#include "Image.h"


string create_name(const string &name, int num) {
    string new_name, ext;
    int pos = name.size();
    for(int i = 0; i < name.size(); i++) {
        if(name[i] == '.') {
            pos = i;
        }
    }
    new_name = name.substr(0, pos) + "_" + to_string(num) + "." + name.substr(pos + 1, name.size() - pos);
    return new_name;
}


int main(int argc, char **argv) {
    try {
        auto args = Args(argc, argv);
        Image img{};
        if(args.input_count() == 3) {
            img.read(
                create_name(args.input_file(), 1),
                create_name(args.input_file(), 2),
                create_name(args.input_file(), 3)
                    );
        } else {
            img.read(args.input_file());
        }
        img.convert(args.from(), args.to());
        if(args.output_count() == 3) {
            img.write(
                create_name(args.output_file(), 1),
                create_name(args.output_file(), 2),
                create_name(args.output_file(), 3)
                    );
        } else {
            img.write(args.output_file());
        }
    }
    catch(std::invalid_argument &ia) {
        std::cerr << "Error while parsing arguments\n";
        std::cerr << ia.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    catch(std::runtime_error &re) {
        std::cerr << "Error during runtime\n";
        std::cerr << re.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
