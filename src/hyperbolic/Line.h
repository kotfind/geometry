#pragma once

#include "Point.h"

#include "core/GeometryObject.h"

#include "euclidian/Point.h"
#include "euclidian/Line.h"
#include "euclidian/Circle.h"
#include "euclidian/Arc.h"

#include <tuple>
#include <optional>

namespace hyperbolic::impl {
    class Point;
    using EPoint = euclidian::impl::Point;
    using ELine = euclidian::impl::Line;
    using ECircle = euclidian::impl::Circle;
    using EArc = euclidian::impl::Arc;

    class Line : public GeometryObject {
        public:
            Line();
            Line(const Point& p1, const Point& p2);

            enum { Type = 1 << 2 };
            int getType() const override { return Type; }

            Object* clone() const override;

            void paint(QPainter* painter, double scale, const QColor& color) const override;
            QRectF boundingRect(double scale) const override;
            QPainterPath shape(double scale) const override;

            void transform(const AbstractTransformation*) override;

            AbstractPoint* calcNearestPoint(const AbstractPoint* pos) const override;

            double pointToPosValue(const AbstractPoint* pos) const override;
            AbstractPoint* posValueToPoint(double val) const override;

            // Returns a, b, c from equation Ax + By + C = 0
            // A^2 + B^2 > C^2
            std::tuple<double, double, double> getABC() const;

            Point p1;
            Point p2;

        private:
            // Retruns EArc which represents
            // current line on poincare disk
            EArc toPoincare() const;
    };

    std::pair<Point, Point> getIntersectionsWithAbsolute(double a, double b, double c);
    std::pair<Point, Point> getTwoPointsOnLine(double a, double b, double c);

    Line perp(const Line& l, const Point& p);
    Line perp(const Point& p, const Line& l);

    std::optional<Point> intersect(const Line& l1, const Line& l2);
}
