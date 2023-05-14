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

        std::pair<QPointF, QPointF> getTwoPoints() const;

        // Returns (A, B, C) for line equation: Ax + By + C = 0
        std::tuple<double, double, double> getABC() const;

        void paint(QPainter* painter) const override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        GeometryObject* transformed(const Transformation&) const;

        QPointF calcNearestPoint(const QPointF& pos) const;

        QPointF getNorm() const;

    private:
        QPointF p1;
        QPointF p2;

        static constexpr double paintWidth = 3e-3;
};
