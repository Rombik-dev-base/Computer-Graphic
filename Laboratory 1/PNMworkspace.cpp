//
// Created by 1285166 on 06.03.2020.
//

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
    if(Version != 5 && Version != 6){
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
    if(Version == 5){
        Factor = 1;
        data.resize(Width * Height);
        input.read((char*) &data[0], Width * Height);
    }
    else{
        Factor = 3;
        data.resize(Width*Height*3);
        input.read((char*) &data[0], Width*Height*3);
    }
    input.close();
    switch (Command){
        case 0:
            CollorsInversion();
            break;
        case 1:
            HorizontalMirror();
            break;
        case 2:
            VerticalMirror();
            break;
        case 3:
            Turn90s();
            break;
        case 4:
            Turn270s();
            break;
    }
}

void PNMworkspace :: CollorsInversion() {
    for(int i = 0; i < data.size(); i++){
        data[i] = Collors - data[i];
    }
}

void PNMworkspace :: HorizontalMirror() {
    for(int i = 0 ; i < Height; i++){
        for(int j = 0; j < (Width / 2); j ++) {
            if (Factor == 3)
                for (int z = 0; z < 3; z++) {
                    swap(data[(i * Width  + j) * Factor + z], data[(i * Width + Width - j - 1 ) * Factor + z]);
                }
            else
                swap(data[(i * Width + j) * Factor], data[(i * Width + Width - j - 1) * Factor]);
        }
    }
}

void PNMworkspace :: VerticalMirror() {
    for(int i = 0 ; i < Height / 2; i++){
        for(int j = 0; j < Width; j++) {
            if(Factor == 3)
            for (int z = 0; z < 3; z++) {
                swap(data[(i * Width + j) * Factor + z], data[(Width * (Height - 1) - i * Width + j) * Factor + z]);
            }
            else
                swap(data[(i * Width + j) * Factor ], data[(Width * (Height - 1) - i * Width + j) * Factor ]);
        }
    }
}

void PNMworkspace ::Turn90s() {
    vector<unsigned char> Turn(Width*Height*Factor);
    for(int i = 0; i < Height; i++){
        for(int j = 0; j < Width; j++){
            if(Factor == 3)
                for(int z = 0; z < 3; z++){
                    Turn[((Height - i - 1) + j * Height) * Factor + z] = data[(i * Width + j) * Factor + z];
                }
            else
                Turn[((Height - i - 1) + j * Height) * Factor] = data[(i * Width + j) * Factor];
        }
    }
    swap(Height,Width);
    data.clear();
    data = Turn;
    Turn.clear();
}

void PNMworkspace :: Turn270s(){
    vector<unsigned char> Turn(Width*Height*Factor);
    for(int i = 0; i < Height; i++){
        for(int j = 0; j < Width; j++){
            if(Factor == 3)
                for(int z = 0; z < 3; z++){
                    Turn[(Height * (Width - j - 1) + i) * Factor + z] = data[(i * Width + j) * Factor + z];
                }
            else
                Turn[(Height * (Width - j - 1) + i) * Factor] = data[(i * Width + j) * Factor];
        }
    }
    swap(Height,Width);
    data.clear();
    data = Turn;
    Turn.clear();
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
