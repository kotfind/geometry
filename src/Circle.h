#pragma once

#include "GeometryObject.h"
#include "Point.h"

class Circle : public GeometryObject {
    public:
        int getType() const { return type; }
        static const int type = 4;

        Circle();
        Circle(const Point& o, double r);

        void paint(QPainter* painter) const override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        const double paintWidth = 2;
        
        Point o;
        double r;
};
