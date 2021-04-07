#include <iostream>
#include "picture.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 7) {
        cerr << "Incorrect count of arguments" << endl;
        return 1;
    }

    bool gradient;
    int gr =  atoi(argv[3]);

    if (gr == 1) {
        gradient = true;
    }
    else if (gr == 0 )
        gradient = false;
    else {
        std::cerr << "Gradient must by 1 or 0" << endl;
        return 1;
    }

    int algo = atoi(argv[4]);

    if (algo > 7) {
        std::cerr << "Dithering algorithm must be integer from 0 to 7" << endl;
        return 1;
    }

    dith_alg algoritm = (dith_alg) (algo);
    int bit_rate = atoi(argv[5]);
    Picture picture;
    picture.set_gamma(atof(argv[6]));
    picture.read(argv[1]);

    if (gradient)
        picture.gradient();

    picture.dither(algoritm, bit_rate);
    picture.write(argv[2]);
    return 0;
}
