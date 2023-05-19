#pragma once

#include "core/GeometryObject.h"

#include <tuple>
#include <QPointF>

namespace euclidian::impl {
    class Line;
    class Point;

    // Line with equation a*x + b*y + c = 0
    class Line : public GeometryObject {
        public:
            Line();
            Line(const QPointF& p1, const QPointF& p2);
            Line(const Point& p1, const Point& p2);

            enum { Type = 1 << 2 };
            int getType() const override { return Type; }

            Object* clone() const override;

            void paint(QPainter* painter, const QColor& color) const override;
            QRectF boundingRect() const override;
            QPainterPath shape() const override;

            GeometryObject* transformed(const AbstractTransformation*) const override;

            QPointF calcNearestPoint(const QPointF& pos) const override;

            double pointToPosValue(const QPointF& pos) const override;
            QPointF posValueToPoint(double val) const override;

            std::pair<Point, Point> getTwoPoints() const;

            // Returns (A, B, C) for line equation: Ax + By + C = 0
            std::tuple<double, double, double> getABC() const;

            QPointF getDir() const;

            QPointF getNorm() const;

            double getDist(const QPointF&) const;

        private:
            std::pair<QPointF, QPointF> getTwoBoundingPoints() const;

            QPointF p1;
            QPointF p2;

            static constexpr double paintWidth = 3e-3;
    };

    double dist(const Line& l, const Point& p);
    double dist(const Point& p, const Line& l);

    Point norm(const Line& l);
    Point dir(const Line& l);
}
