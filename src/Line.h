#pragma once

#include "GeometryObject.h"

// Line with equation a*x + b*y + c = 0
class Line : public GeometryObject {
    public:
        int getType() const { return type; }
        static const int type = 3;

        Line();
        Line(double a, double b, double c);

        void paint(QPainter* painter) const override;
        QRectF boundingRect() const override;

        double a, b, c;
};
