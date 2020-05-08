#include <iostream>
#include <fstream>
#include "PNMworkspace.h"

using namespace std;



int main(int argc, char* argv[]){
    if(argc != 4){
        cout << "Wrong input data, check manual\n";
        return 0;
    }
    int command = stoi(argv[3]);
    if(command < 0 || command > 4){
        cout << "Wrong command, check manual about commands\n";
        return 0;
    }

    PNMworkspace newwork(argv[1],argv[2], command);
    newwork.readfile();
    newwork.Write();
    return 0;
}
