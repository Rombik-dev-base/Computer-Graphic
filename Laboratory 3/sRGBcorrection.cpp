//
// Created by 1285166 on 25.04.2020.
//

#include "sRGBcorrection.h"


double correctValue(double k) {
    return k <= 0.0031308 ? 12.92 * k : 1.055 * pow(k, 1 / 2.4) - 0.055;
}

double LinerValueCorrection(double k) {
    return k <= 0.04045 ? k / 12.92 : pow((k + 0.055) / 1.055, 2.4);
}

uchar ToLinerColorCorrection(uchar color) {
    double colorSRGB = color / 255.0;
    double colorLinear = LinerValueCorrection(colorSRGB);
    return colorLinear * 255;
}

uchar correctColor(uchar color) {
    double colorLinear = color / 255.0;
    double colorSRGB = correctValue(colorLinear);
    return colorSRGB * 255;
}
