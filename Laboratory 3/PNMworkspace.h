//
// Created by 1285166 on 25.04.2020.
//

#ifndef INC_3RDCOMPGRAPHIC_PNMWORKSPACE_H
#define INC_3RDCOMPGRAPHIC_PNMWORKSPACE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <time.h>
#include "sRGBcorrection.h"

using namespace std;

typedef unsigned char uchar;

class myexception : public exception {
public:
    myexception(char *omg){
        excepinfo = omg;
    }
    const char* what() const throw() override{
        return excepinfo;
    }
    char *excepinfo;
};

enum DitherAlgo {
    NONE = 0,
    ORDERED = 1,
    RANDOM = 2,
    FLOYD_STEINBERG = 3,
    JJN = 4,
    SIERRA = 5,
    ATKINSON = 6,
    HALFTONE = 7
};

struct Point {
    float x;
    float y;
};

class PNMworkspace {
public:
    PNMworkspace();
    explicit PNMworkspace(string fileName);

    void read(string fileName);
    void read(ifstream& inputFile);

    void write(const string& fileName);
    void write(ofstream& outputFile);

    void fillWithGradient();

    void dither(DitherAlgo algo, uchar bitRate);

private:
    short format;
    int width, height;
    int colors;
    vector<uchar> data;

    uchar& get(int h, int w);

    void ditherNone(uchar bitRate);
    void ditherOrdered(uchar bitRate);
    void ditherRandom(uchar bitRate);
    void ditherFloydSteinberg(uchar bitRate);
    void ditherJJN(uchar bitRate);
    void ditherSierra(uchar bitRate);
    void ditherAtkinson(uchar bitRate);
    void ditherHalftone(uchar bitRate);


};


#endif //INC_3RDCOMPGRAPHIC_PNMWORKSPACE_H
