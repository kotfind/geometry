#pragma once

#include "Point.h"
#include "Line.h"

#include "euclidian/Circle.h"

#include "core/GeometryObject.h"

namespace hyperbolic::impl {
    using EPoint = euclidian::impl::Point;
    using ECircle = euclidian::impl::Circle;

    class Circle : public GeometryObject {
        public:
            Circle();
            Circle(const Point& o, double r);

            enum { Type = 1 << 3 };
            int getType() const override { return Type; }

            Object* clone() const override;

            void paint(QPainter* painter, const QColor& color) const override;
            QRectF boundingRect() const override;
            QPainterPath shape() const override;

            void transform(const AbstractTransformation*) override;

            AbstractPoint* calcNearestPoint(const AbstractPoint* pos) const override;

            double pointToPosValue(const AbstractPoint* pos) const override;
            AbstractPoint* posValueToPoint(double val) const override;

            std::tuple<Point, Point, Point> getThreePoints() const;

            // Return a, b, c as in equation
            //     sqrt(1 - x*x - y*y) = a*x + b*y + c
            std::tuple<double, double, double> getABC() const;

            Point o;
            double r;
            
        private:
            // Retruns ECircle which represents
            // current circle on poincare disk
            ECircle toPoincare() const;
    };
}
