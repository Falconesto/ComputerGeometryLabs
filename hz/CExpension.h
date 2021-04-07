//
// Created by Санжар Муслимов on 20.04.20.
//

#ifndef GRAPH_CEXPENSION_H
#define GRAPH_CEXPENSION_H

#include <string>
using namespace std;

class CExpension {
public:
    CExpension(string error);

    CExpension(string error, FILE *file);

    string getError();

    FILE *getFile();

private:
    string error_;
    FILE *file_ = nullptr;
};


#endif //GRAPH_CEXPENSION_H
