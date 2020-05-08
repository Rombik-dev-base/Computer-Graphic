//
// Created by 1285166 on 24.04.2020.
//

#include "PNMredactor.h"
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include "myexceptions.h"
#define _CRT_SECURE_NO_DEPRECATE
using namespace std;

Image::Image()
{
    height = 0;
    width = 0;
    image;
}
Image::~Image()
{
    image.clear();
}
void Image::Read(char* file)
{
    FILE* f = fopen(file, "rb");
    if (f == nullptr)
        throw myexception("ERROR. File not open");
    if (fscanf(f, "%c%c\n%zu %zu\n%zu\n", &format[0], &format[1], &width, &height, &grade) != 5)
        throw myexception("ERROR. Bad header");
    if (format[1] - '0' != 5)
        throw myexception("ERROR. Unknown format");
    if (width < 0 || height < 0 || grade != 255)
        throw myexception("ERROR. Bad values");
    size = width * height;
    uchar* buffer = new uchar[size];
    fread(buffer, sizeof(uchar), size, f);
    fclose(f);
    size = width * height;
    image.resize(size);
    for (size_t i = 0; i < size; i++)
        image[i] = buffer[i];
    delete[] buffer;
}



void Image::Write(char* file)
{
    FILE* f = fopen(file, "wb");
    if (f == nullptr)
        throw myexception("ERROR. File not open");
    if (fprintf(f, "%c%c\n%zu %zu\n%d\n", format[0], format[1], width, height, grade) != 15)
        throw myexception("ERROR. Writing header problem");
    uchar* buffer = new uchar[size];
    for (size_t i = 0; i < size; i++)
        buffer[i] = image[i];
    fwrite(buffer, sizeof(uchar), size, f);
    delete[] buffer;
    fclose(f);
}



void Image::DrawPoint(double x, double y, double transparency, double brightness, double gamma) {
    transparency = max(min(transparency, 1.0), 0.0);
    if (y < 0 || y >= height || x < 0 || x >= width)
        throw myexception("ERROR. Bad coords");
    image[width * y + x] = 255 * pow((transparency * image[width * y + x] + brightness * (1 - transparency)) / 255.0, (1.0 / gamma - 1.0) * (1.0 - transparency) + 1.0);
}




void Image::DrawPoint(double x, double y, double transparency, double brightness) {
    transparency = max(min(transparency, 1.0), 0.0);
    if (y < 0 || y >= height || x < 0 || x >= width)
        throw myexception("ERROR. Bad coords");
    const double lineColor = brightness / 255.0;
    const double sRGBColor = image[y * width + x] / 255.0;
    const double imageLineColor = sRGBColor <= 0.04045 ? sRGBColor / 12.92 : pow((sRGBColor + 0.055) / 1.055, 2.4);
    const double c = (1 - transparency) * lineColor + transparency * imageLineColor;
    const double sRGBc = c <= 0.0031308 ? 12.92 * c : 1.055 * pow(c, 1 / 2.4) - 0.055;
    image[width * y + x] = 255 * sRGBc;
}


void Image::DrawLine(Line line) {
    if (line.thickness <= 0)
        throw myexception("ERROR. wtf");
    bool steep = abs(line.end.y - line.begin.y) > abs(line.end.x - line.begin.x);
    auto intPart = [](double value) -> int
    {
        return static_cast<int>(value);
    };
    auto plot = [&](double x, double y, double intensity) -> void {
        if (line.gamma == 0) {
            if (steep)
                DrawPoint(y, x, 1.0 - intensity, line.brightness);
            else
                DrawPoint(x, y, 1.0 - intensity, line.brightness);
        }
        else {
            if (steep)
                DrawPoint(y, x, 1.0 - intensity, line.brightness, line.gamma);
            else
                DrawPoint(x, y, 1.0 - intensity, line.brightness, line.gamma);
        }
    };
    auto distance = [](Point a, Point b) -> double {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    };
    if (steep) {
        swap(line.begin.x, line.begin.y);
        swap(line.end.x, line.end.y);
    }
    if (line.begin.x > line.end.x) {
        swap(line.begin.x, line.end.x);
        swap(line.begin.y, line.end.y);
    }
    const double dx = line.end.x - line.begin.x;
    const double dy = line.end.y - line.begin.y;
    const double gradient = dy / dx;
    double y = line.begin.y + gradient * (round(line.begin.x) - line.begin.x);
    for (size_t x = round(line.begin.x); x <= round(line.end.x); x++) {
        for (size_t plotY = intPart(y - (line.thickness - 1) / 2); plotY <= intPart(y - (line.thickness - 1) / 2 + line.thickness); plotY++)
        {
            plot(x, plotY, min(1.0, (line.thickness + 1.0) / 2.0 - fabs(y - plotY)));
        }
        y += gradient;
    }
    const Point plotStart = { round(line.begin.x), round(line.begin.y) };
    const Point plotEnd = { round(line.end.x), round(line.end.y) };
    for (size_t plotX = round(line.begin.x) - line.thickness / 2; plotX < round(line.begin.x); plotX++) {
        y = line.begin.y + gradient * (plotX - line.begin.x);
        for (int plotY = static_cast<int>(y - (line.thickness - 1) / 2.0); plotY <= static_cast<int>(y - (line.thickness - 1) / 2.0 + line.thickness); plotY++)
            plot(plotX, plotY, min(1.0, (line.thickness + 0.5) / 2.0 - distance({ static_cast<double>(plotX), static_cast<double>(plotY) }, { plotStart.x, plotStart.y })));
    }
    for (size_t plotX = round(line.end.x) + 1; plotX <= round(line.end.x) + line.thickness / 2; plotX++) {
        y = line.begin.y + gradient * (plotX - line.begin.x);
        for (int plotY = int(y - (line.thickness - 1) / 2.0); plotY <= int(y - (line.thickness - 1) / 2.0 + line.thickness); plotY++) {
            plot(plotX, plotY, min(1.0, (line.thickness + 0.5) / 2.0 - distance({ static_cast<double>(plotX), static_cast<double>(plotY) }, { plotEnd.x, plotEnd.y })));
        }
    }
}
