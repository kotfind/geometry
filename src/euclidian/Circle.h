#pragma once

#include "Point.h"
#include "Line.h"

#include "core/GeometryObject.h"

#include <tuple>

namespace euclidian::impl {
    class Circle : public GeometryObject {
        public:
            Circle();
            Circle(const Point& o, double r);

            enum { Type = 1 << 3 };
            int getType() const override { return Type; }

            Object* clone() const override;

            void paint(QPainter* painter, double scale, const QColor& color) const override;
            QRectF boundingRect(double scale) const override;
            QPainterPath shape(double scale) const override;

            void transform(const AbstractTransformation*) override;

            AbstractPoint* calcNearestPoint(const AbstractPoint* pos) const override;

            // posValue for Circle is angle
            double pointToPosValue(const AbstractPoint* pos) const override;
            AbstractPoint* posValueToPoint(double val) const override;

            std::tuple<Point, Point, Point> getThreePoints() const;

            Point o;
            double r;
    };

    void intersect(
        const Circle& w1,
        const Circle& w2,
        int& n,
        Point& p1,
        Point& p2
    );

    void intersect(
        const Line& l,
        const Circle& w,
        int& n,
        Point& p1,
        Point& p2
    );

    void intersect(
        const Circle& w,
        const Line& l,
        int& n,
        Point& p1,
        Point& p2
    );
}
