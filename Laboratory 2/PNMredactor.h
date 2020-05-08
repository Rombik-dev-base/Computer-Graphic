//
// Created by 1285166 on 24.04.2020.
//

#ifndef INC_2NDLABCOMPUTERGRAPHIC_PNMREDACTOR_H
#define INC_2NDLABCOMPUTERGRAPHIC_PNMREDACTOR_H

#include <vector>

typedef unsigned char uchar;


class Point
{
public:
    Point() = default;
    double x;
    double y;
};
class Line
{
public:
    Line(Point _begin, Point _end, double _thickness, int _brightness, double _gamma);
    Point begin{};
    Point end{};
    double thickness;
    int brightness;
    double gamma;
};
class Image
{
public:
    Image();
    ~Image();
    void Read(char*);
    void Write(char*);
    void DrawPoint(double x, double y, double transparency, double brightness);
    void DrawPoint(double x, double y, double transparency, double brightness, double gamma);
    void DrawLine(Line line);
private:
    char format[2];
    size_t width;
    size_t height;
    size_t grade;
    size_t size;
    std::vector<uchar> image;
};

#endif //INC_2NDLABCOMPUTERGRAPHIC_PNMREDACTOR_H
