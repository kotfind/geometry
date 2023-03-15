#pragma once

#include "GeometryObject.h"

class QPointF;

class Point : public GeometryObject {
    public:
        int getType() const { return type; }
        static const int type = 2;

        Point();
        Point(const QPointF& pos);
        Point(double x, double y);

        void paint(QPainter* painter) const override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        void setPos(const QPointF& pos);
        void move(const QPointF& delta);

        QPointF getPos() const;

        double x, y;

        static constexpr double paintRadius = 5e-3;
};

bool operator==(const Point& p1, const Point& p2);

double dist(const Point& p1, const Point& p2);
