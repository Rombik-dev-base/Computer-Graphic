#include <iostream>
#include "PNMredactor.h"
#include "myexceptions.h"
#include "information.h"
using namespace std;

int main(int argc, char** argv) {
    try {
        Input consoleInput;
        Image image;
        if (argc < 9)
            throw myexception("ERROR. Bad argument count");
        for (size_t i = 1; i < argc; i++) {
            switch (i)
            {
                case INPUT:
                    consoleInput.fileInput = argv[i];
                    break;
                case OUTPUT:
                    consoleInput.fileOutput = argv[i];
                    break;
                case BRIGHTNESS:
                    consoleInput.brightness = atoi(argv[i]);
                    break;
                case THICKNESS:
                    consoleInput.thickness = atof(argv[i]);
                    break;
                case BEGIN_POINT_X:
                    consoleInput.first.x = atof(argv[i]);
                    break;
                case BEGIN_POINT_Y:
                    consoleInput.first.y = atof(argv[i]);
                    break;
                case END_POINT_X:
                    consoleInput.second.x = atof(argv[i]);
                    break;
                case END_POINT_Y:
                    consoleInput.second.y = atof(argv[i]);
                    break;
                case GAMMA:
                    consoleInput.gamma = atof(argv[i]);
                    break;
                default:
                    throw myexception("ERROR. Bad argument count");
            }
        }
        const Line lineSettings = { consoleInput.first, consoleInput.second, consoleInput.thickness, consoleInput.brightness, consoleInput.gamma };
        image.Read(consoleInput.fileInput);
        image.DrawLine(lineSettings);
        image.Write(consoleInput.fileOutput);
    }
    catch (const exception & error) {
        cerr << error.what() << endl;
        return -1;
    }
    return 0;
}
