#pragma once

#include "GeometryObject.h"
#include <QDebug>

class Point : public GeometryObject {
    public:
        int getType() const { return type; }
        static const int type = 2;

        Point();
        Point(double x, double y);

        void paint(QPainter* painter) const override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        void setPos(double x, double y);

        double x, y;

        static constexpr double paintRadius = 3;
};

bool operator==(const Point& p1, const Point& p2);
