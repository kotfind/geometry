#pragma once

#include "GeometryObject.h"

#include <QPointF>
#include <QPainter>

class Point;

class Segment : public GeometryObject {
    public:
        Segment();
        Segment(const QPointF& p1, const QPointF& p2);
        Segment(const Point& p1, const Point& p2);

        Type getType() const override { return Type::Segment; }

        Object* clone() const override;

        void paint(QPainter* painter) const override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        GeometryObject* transformed(const Transformation&) const override;

        QPointF calcNearestPoint(const QPointF& pos) const override;

        std::pair<QPointF, QPointF> getTwoPoints() const;
        std::pair<Point, Point> getTwoPoints_() const;

        // Returns (A, B, C) for line equation: Ax + By + C = 0
        std::tuple<double, double, double> getABC() const;

        QPointF getNorm() const;

        double getDist(const QPointF&) const;

    private:
        QPointF p1;
        QPointF p2;

        static constexpr double paintWidth = 3e-3;
};

double dist(const Segment& s, const Point& p);
double dist(const Point& p, const Segment& s);

Point norm(const Segment& s);
Point dir(const Segment& s);
