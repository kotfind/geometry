#pragma once

#include "GeometryObject.h"

// Line with equation a*x + b*y + c = 0
class Line : public GeometryObject {
    public:
        int getType() const { return type; }
        static const int type = 3;

        Line() : Line(0, 1, 0) {}
        Line(double a, double b, double c) : GeometryObject(), a(a), b(b), c(c) {}

        void paint() const { qDebug("Paint line"); }

        double a, b, c;
};
