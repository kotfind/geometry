#pragma once

#include "GeometryObject.h"
#include "Point.h"

class Circle : public GeometryObject {
    public:
        Type getType() const { return Type::Line; }

        Circle();
        Circle(const Point& o, double r);

        void paint(QPainter* painter) const override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        static constexpr double paintWidth = 3e-3;
        
        Point o;
        double r;
};
