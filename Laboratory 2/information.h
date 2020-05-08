//
// Created by 1285166 on 24.04.2020.
//

#ifndef INC_2NDLABCOMPUTERGRAPHIC_INFORMATION_H
#define INC_2NDLABCOMPUTERGRAPHIC_INFORMATION_H

enum PARAMETRS {
    INPUT = 1,
    OUTPUT,
    BRIGHTNESS,
    THICKNESS,
    BEGIN_POINT_X,
    BEGIN_POINT_Y,
    END_POINT_X,
    END_POINT_Y,
    GAMMA,
};

struct Input {
    char* fileInput = nullptr;
    char* fileOutput = nullptr;
    int brightness = 0;
    double thickness = 0;
    Point first = { 0, 0 };
    Point second = { 0, 0 };
    double gamma = 0;
};

Line::Line(Point _begin, Point _end, double _thickness, int _brightness, double _gamma)
{
    begin.x = _begin.x;
    end.x = _end.x;
    begin.y = _begin.y;
    end.y = _end.y;
    thickness = _thickness;
    brightness = _brightness;
    gamma = _gamma;
}


#endif //INC_2NDLABCOMPUTERGRAPHIC_INFORMATION_H
