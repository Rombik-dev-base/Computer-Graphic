//
// Created by 1285166 on 06.03.2020.
//

#ifndef COMPGEOMLAB1_PNMWORKSPACE_H
#define COMPGEOMLAB1_PNMWORKSPACE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>

using namespace std;

class PNMworkspace{
public:
    PNMworkspace(string inpFileName, string outpFileName, int command)
    :InpFileName(inpFileName)
    ,OutpFileName(outpFileName)
    ,Command(command)
    {};
    void readfile();
    void Write();
private:
    string InpFileName;
    string OutpFileName;
    int Command, Collors, Version, Width, Height, Factor;
    vector<unsigned char> data;
};

#endif //COMPGEOMLAB1_PNMWORKSPACE_H
