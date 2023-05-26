#pragma once

#include "Point.h"

#include "core/GeometryObject.h"

#include "euclidian/Point.h"
#include "euclidian/Line.h"
#include "euclidian/Circle.h"

#include <tuple>

namespace hyperbolic::impl {
    class Point;
    using EPoint = euclidian::impl::Point;
    using ELine = euclidian::impl::Line;
    using ECircle = euclidian::impl::Circle;

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

            double pointToPosValue(const AbstractPoint* pos) const override;
            AbstractPoint* posValueToPoint(double val) const override;

            // Returns a, b, c from equation Ax + By + C = 0
            // A^2 + B^2 > C^2
            std::tuple<double, double, double> getABC() const;

            Point p1;
            Point p2;

        private:
            // Retruns GeometryObject which is either ECircle or ELine
            GeometryObject* getEuclidian() const;
    };
}
