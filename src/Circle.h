#pragma once

#include "GeometryObject.h"
#include "Point.h"

class Circle : public GeometryObject {
    public:
        int getType() const { return type; }
        static const int type = 4;

        Circle() : Circle(Point(), 1) {}
        Circle(const Point& o, double r) : GeometryObject(), o(o), r(r) {}

        void paint() const { qDebug("Paint circle"); }
        
        Point o;
        double r;
};
