#ifndef GRAPH_CLINE_H
#define GRAPH_CLINE_H


struct SPoint {
    double x_;
    double y_;
};

struct SInput {
    const char *inputName;
    const char *outputName;
    int brightness;
    double thickness;
    SPoint start;
    SPoint end;
    double gamma;
};

class CLine {
public:
    CLine(SInput input);
    int brightness;
    double thickness;
    SPoint start_;
    SPoint end_;
    double gamma_ = 2.2;
};


#endif //GRAPH_CLINE_H
