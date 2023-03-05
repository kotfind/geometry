#pragma once

#include "GeometryObject.h"
#include <QDebug>

class Point : public GeometryObject {
    public:
        int getType() const { return type; }
        static const int type = 2;

        Point() : Point(0, 0) {}
        Point(double x, double y) : GeometryObject(), x(x), y(y) {}

        void paint() const { qDebug("Paint point"); }

        double x, y;
};

bool operator==(const Point& p1, const Point& p2);