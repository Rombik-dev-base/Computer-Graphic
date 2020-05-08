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
    void CollorsInversion();
    void HorizontalMirror();
    void VerticalMirror();
    void Turn90s();
    void Turn270s();
    void Write();
private:
    string InpFileName;
    string OutpFileName;
    int Command, Collors, Version, Width, Height, Factor;
    vector<unsigned char> data;
};
