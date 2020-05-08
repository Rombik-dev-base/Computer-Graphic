#include "PNMworkspace.h"

void PNMworkspace :: readfile(){
    ifstream input(InpFileName, ios_base::binary);
    if(!input.is_open()){
        cout << "File is not found, try to open other file" << endl;
        exit(0);
    }
    char format;
    input >> format;
    if(format != 'P'){
        cout << "Wrong file format, try to use other file";
        exit(0);
    }
    input >> Version;
    if(Version != 5){
        cout << "Wrong file version, use file with P5 or P6 version";
        exit(0);
    }
    input >> Width >> Height;
    input >> Collors;
    if(Collors != 255){
        cout << "Wrong collor format, use 255 collors format file";
        exit(0);
    }
    input.get();

    data.resize(Width * Height);
    input.read((char*) &data[0], Width * Height);
    input.close();
}

void PNMworkspace :: Write() {
    ofstream out(OutpFileName, ios_base::binary);
    out << "P" << Version << "\n";
    out << Width << ' ' << Height << endl;
    out << Collors << endl;
    out.write((char*) &data[0], Width*Height*Factor);
    data.clear();
    out.close();
}
