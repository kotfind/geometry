#pragma once

#include "GeometryObject.h"

#include <QPair>

class Point;
class QPointF;

// Line with equation a*x + b*y + c = 0
class Line : public GeometryObject {
    public:
        Type getType() const { return Type::Line; }

        Line();
        Line(double a, double b, double c);
        Line(const Point& p1, const Point& p2);

        void paint(QPainter* painter) const override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        QPair<Point, Point> getTwoPoints() const;

        double a, b, c;

        static constexpr double paintWidth = 3e-3;

    private:
        QPair<QPointF, QPointF> getTwoQPointFs() const;

        QPointF getNorm() const;
};

double dist(const Line& l, const Point& p);
double dist(const Point& p, const Line& l);

Point norm(const Line& l);
Point dir(const Line& l);
