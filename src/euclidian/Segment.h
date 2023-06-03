#pragma once

#include "Point.h"
#include "Line.h"

#include "core/GeometryObject.h"

#include <tuple>
#include <QPointF>

namespace euclidian::impl {
    class Point;

    class Segment : public GeometryObject {
        public:
            Segment();
            Segment(const Point& p1, const Point& p2);

            enum { Type = 1 << 4 };
            int getType() const override { return Type; }

            Object* clone() const override;

            void paint(QPainter* painter, const QColor& color) const override;
            QRectF boundingRect() const override;
            QPainterPath shape() const override;

            void transform(const AbstractTransformation*) override;

            AbstractPoint* calcNearestPoint(const AbstractPoint* pos) const override;

            double pointToPosValue(const AbstractPoint* pt) const override;
            AbstractPoint* posValueToPoint(double val) const override;

            std::tuple<double, double, double> getABC() const;

            Line toLine() const;

            Point p1;
            Point p2;

        private:
            static constexpr double paintWidth = 7e-3;
    };

    Point norm(const Segment& l);
    Point dir(const Segment& l);

    bool on(const Point& p, const Segment& s);
}
