//
// Created by 1285166 on 25.04.2020.
//

#ifndef INC_3RDCOMPGRAPHIC_SRGBCORRECTION_H
#define INC_3RDCOMPGRAPHIC_SRGBCORRECTION_H
#include "PNMworkspace.h"

typedef unsigned char uchar;

double correctValue(double k);
double LinerValueCorrection(double k);
uchar ToLinerColorCorrection(uchar color);
uchar correctColor(uchar color);

#endif //INC_3RDCOMPGRAPHIC_SRGBCORRECTION_H
