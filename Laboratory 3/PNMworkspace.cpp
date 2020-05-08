//
// Created by 1285166 on 25.04.2020.
//

#include "PNMworkspace.h"
#include "sRGBcorrection.h"



const double orderedMatrix[8][8] = {
        {0.0 / 64.0, 48.0 / 64.0, 12.0 / 64.0, 60.0 / 64.0, 3.0 / 64.0, 51.0 / 64.0, 15.0 / 64.0, 63.0 / 64.0},
        {32.0 / 64.0, 16.0 / 64.0, 44.0 / 64.0, 28.0 / 64.0, 35.0 / 64.0, 19.0 / 64.0, 47.0 / 64.0, 31.0 / 64.0},
        {8.0 / 64.0, 56.0 / 64.0, 4.0 / 64.0, 52.0 / 64.0, 11.0 / 64.0, 59.0 / 64.0, 7.0 / 64.0, 55.0 / 64.0},
        {40.0 / 64.0, 24.0 / 64.0, 36.0 / 64.0, 20.0 / 64.0, 43.0 / 64.0, 27.0 / 64.0, 39.0 / 64.0, 23.0 / 64.0},
        {2.0 / 64.0, 50.0 / 64.0, 14.0 / 64.0, 62.0 / 64.0, 1.0 / 64.0, 49.0 / 64.0, 13.0 / 64.0, 61.0 / 64.0},
        {34.0 / 64.0, 18.0 / 64.0, 46.0 / 64.0, 30.0 / 64.0, 33.0 / 64.0, 17.0 / 64.0, 45.0 / 64.0, 29.0 / 64.0},
        {10.0 / 64.0, 58.0 / 64.0, 6.0 / 64.0, 54.0 / 64.0, 9.0 / 64.0, 57.0 / 64.0, 5.0 / 64.0, 53.0 / 64.0},
        {42.0 / 64.0, 26.0 / 64.0, 38.0 / 64.0, 22.0 / 64.0, 41.0 / 64.0, 25.0 / 64.0, 37.0 / 64.0, 21.0 / 64.0}
};

const double matrixJJN[3][5] = {
        {0, 0, 0, 7.0 / 48.0, 5.0 / 48.0},
        {3.0 / 48.0, 5.0 / 48.0, 7.0 / 48.0, 5.0 / 48.0, 3.0 / 48.0},
        {1.0 / 48.0, 3.0 / 48.0, 5.0 / 48.0, 3.0 / 48.0, 1.0 / 48.0}
};

const double matrixSierra3[3][5] = {
        {0, 0, 0, 5.0 / 32.0, 3.0 / 32.0},
        {2.0 / 32.0, 4.0/ 32.0, 5.0 / 32.0, 4.0 / 32.0, 2.0 / 32.0},
        {0, 2.0 / 32.0, 3.0 / 32.0, 2.0 / 32.0, 0}
};

const int matrixAtkinson[3][5] = {
        {0, 0, 0, 1, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0}
};

const double halftoneMatrix[4][4] = {
        {13.0 / 16.0, 11.0 / 16.0, 4.0 / 16.0, 8.0 / 16.0},
        {6.0 / 16.0, 0, 3.0 / 16.0, 15.0 / 16.0},
        {14.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 7.0 / 16.0},
        {9.0 / 16.0, 5.0 / 16.0, 10.0 / 16.0, 12.0 / 16.0},
};

PNMworkspace ::PNMworkspace() = default;
PNMworkspace ::PNMworkspace(string filename) {
    read(filename);
}

void PNMworkspace ::read(string fileName) {
    ifstream inputFile(fileName, ios::binary);
    if (!inputFile.is_open())
        throw myexception("Error trying to read file ");
    read(inputFile);
    inputFile.close();
}

void PNMworkspace ::read(ifstream& inputFile) {
    char P;
    inputFile >> P;
    if (P != 'P')
        throw myexception( "Error: unsupported format");

    inputFile >> format;
    inputFile >> width >> height;
    inputFile >> colors;
    if (format != 5) {
        cout << format << '\n';
        cout << colors << '\n';
        throw myexception("Error: unsupported format");
    }

    if (colors != 255)
        throw myexception( "Error: unsupported format");

    inputFile.get();

    data.resize(width * height);
        inputFile.read((char*) &data[0], width * height);
    if (inputFile.fail())
        throw myexception("Error trying to write data to file ");
}

void PNMworkspace ::write(const string& fileName) {
    ofstream outputFile(fileName, ios::binary);
    if (!outputFile.is_open())
        throw myexception("Error trying to write data to file ");
    write(outputFile);
    outputFile.close();
}

void PNMworkspace ::write(ofstream& outputFile) {
    outputFile << "P" << format << '\n';
    outputFile << width << ' ' << height << '\n';
    outputFile << colors << '\n';
    outputFile.write((char*) &data[0], width * height);
}

void PNMworkspace ::fillWithGradient() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            get(i, j) = correctColor((double) j / width * 255);
        }
    }
}

void PNMworkspace ::dither(DitherAlgo algo, uchar bitRate) {
    switch (algo) {
        case NONE:
            ditherNone(bitRate);
            break;
        case ORDERED:
            ditherOrdered(bitRate);
            break;
        case RANDOM:
            ditherRandom(bitRate);
            break;
        case FLOYD_STEINBERG:
            ditherFloydSteinberg(bitRate);
            break;
        case JJN:
            ditherJJN(bitRate);
            break;
        case SIERRA:
            ditherSierra(bitRate);
            break;
        case ATKINSON:
            ditherAtkinson(bitRate);
            break;
        case HALFTONE:
            ditherHalftone(bitRate);
            break;
        default:
            throw myexception("Error trying to dither the picture" );
    }
}

void PNMworkspace ::ditherNone(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double value = ToLinerColorCorrection(get(i, j)) / 255.0;

            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);
            get(i, j) = correctColor(newPaletteColor / maxValue * 255);
        }
    }
}

void PNMworkspace ::ditherOrdered(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorLinear = LinerValueCorrection(get(i, j) / 255.0);
            double value = picColorLinear + (orderedMatrix[i % 8][j % 8] - 0.5) / bitRate;

            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);
            get(i, j) = correctColor(newPaletteColor / maxValue * 255);
        }
    }
}

void PNMworkspace :: ditherRandom(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    srand(time(NULL));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorLinear = LinerValueCorrection(get(i, j) / 255.0);
            double noise =  (double) rand() / RAND_MAX - 0.75;
            double value = picColorLinear + noise / bitRate;

            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);
            get(i, j) = correctColor(newPaletteColor / maxValue * 255);
        }
    }
}

void PNMworkspace :: ditherFloydSteinberg(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    vector<double> errors(height * width, 0);
    auto getError = [&](int h, int w) -> double& {
        return errors[h * width + w];
    };

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorLinear = LinerValueCorrection(get(i, j) / 255.0);
            double value = picColorLinear + getError(i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);

            double error = ToLinerColorCorrection(get(i, j)) + getError(i, j) - newPaletteColor / (double) maxValue * 255.0;

            get(i, j) = correctColor(newPaletteColor / maxValue * 255);

            if (j + 1 < height)
                getError(i, j + 1) += error * 7.0 / 16.0;
            if (i + 1 < height && j + 1 < height)
                getError(i + 1, j + 1) += error * 1.0 / 16.0;
            if (i + 1 < height)
                getError(i + 1, j) += error * 5.0 / 16.0;
            if (i + 1 < height && j - 1 >= 0)
                getError(i + 1, j - 1) += error * 3.0 / 16.0;
        }
    }
}

void PNMworkspace :: ditherJJN(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    vector<double> errors(height * width, 0);
    auto getError = [&](int h, int w) -> double& {
        return errors[h * width + w];
    };

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorLinear = LinerValueCorrection(get(i, j) / 255.0);
            double value = picColorLinear + getError(i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);

            double error = ToLinerColorCorrection(get(i, j)) + getError(i, j) - newPaletteColor / (double) maxValue * 255.0;

            get(i, j) = correctColor(newPaletteColor / maxValue * 255);

            for (int ie = 0; ie < 3; ie++) {
                for (int je = 0; je < 5; je++) {
                    if (i + ie >= height || j + (je - 2) >= width || j + (je - 2) < 0)
                        break;
                    if (ie == 0 && je <= 2)
                        break;

                    getError(i + ie, j + (je - 2)) += error * matrixJJN[ie][je];
                }
            }
        }
    }
}

void PNMworkspace :: ditherSierra(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    vector<double> errors(height * width, 0);
    auto getError = [&](int h, int w) -> double& {
        return errors[h * width + w];
    };

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorLinear = LinerValueCorrection(get(i, j) / 255.0);
            double value = picColorLinear + getError(i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);

            double error = ToLinerColorCorrection(get(i, j)) + getError(i, j) - newPaletteColor / (double) maxValue * 255.0;

            get(i, j) = correctColor(newPaletteColor / maxValue * 255);

            for (int ie = 0; ie < 3; ie++) {
                for (int je = 0; je < 5; je++) {
                    if (i + ie >= height || j + (je - 2) >= width || j + (je - 2) < 0)
                        break;
                    if (ie == 0 && je <= 2)
                        break;

                    getError(i + ie, j + (je - 2)) += error * matrixSierra3[ie][je];
                }
            }
        }
    }
}

void PNMworkspace :: ditherAtkinson(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    vector<double> errors(height * width, 0);
    auto getError = [&](int h, int w) -> double& {
        return errors[h * width + w];
    };

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorLinear = LinerValueCorrection(get(i, j) / 255.0);
            double value = picColorLinear + getError(i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);

            double error = ToLinerColorCorrection(get(i, j)) + getError(i, j) - newPaletteColor / (double) maxValue * 255.0;

            get(i, j) = correctColor(newPaletteColor / maxValue * 255);

            for (int ie = 0; ie < 3; ie++) {
                for (int je = 0; je < 5; je++) {
                    if (i + ie >= height || j + (je - 2) >= width || j + (je - 2) < 0)
                        break;
                    if (ie == 0 && je <= 2)
                        break;

                    getError(i + ie, j + (je - 2)) += error * matrixAtkinson[ie][je] / 8.0;
                }
            }
        }
    }
}

void PNMworkspace :: ditherHalftone(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorLinear = LinerValueCorrection(get(i, j) / 255.0);
            double value = picColorLinear + (halftoneMatrix[i % 4][j % 4] - 0.75) / bitRate;
            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);

            get(i, j) = round(newPaletteColor / maxValue * 255);
        }
    }
}

uchar& PNMworkspace :: get(int h, int w) {
    if (h < 0 || w < 0 || h >= height || w >= width)
        throw myexception("Out of range");
    return data[width * h + w];
}

