#pragma once

#include "Point.h"

#include "core/GeometryObject.h"

#include <tuple>
#include <QPointF>

namespace euclidian::impl {
    class Point;

    class Line : public GeometryObject {
        public:
            Line();
            Line(const Point& p1, const Point& p2);

            enum { Type = 1 << 2 };
            int getType() const override { return Type; }

            Object* clone() const override;

            void paint(QPainter* painter, const QColor& color) const override;
            QRectF boundingRect() const override;
            QPainterPath shape() const override;

            void transform(const AbstractTransformation*) override;

            AbstractPoint* calcNearestPoint(const AbstractPoint* pos) const override;

            double pointToPosValue(const AbstractPoint* pt) const override;
            AbstractPoint* posValueToPoint(double val) const override;

            // Returns (A, B, C) for line equation: Ax + By + C = 0
            std::tuple<double, double, double> getABC() const;

            Point p1;
            Point p2;

        private:
            std::pair<Point, Point> getTwoBoundingPoints() const;

            static constexpr double paintWidth = 7e-3;
    };

    double dist(const Line& l, const Point& p);
    double dist(const Point& p, const Line& l);

    Point norm(const Line& l);
    Point dir(const Line& l);

    void intersect(const Line& l1, const Line& l2, int& n, Point& p);
}
