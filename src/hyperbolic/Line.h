#pragma once

#include "Point.h"

#include "core/GeometryObject.h"

#include <QPointF>

namespace hyperbolic::impl {
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

            GeometryObject* transformed(const AbstractTransformation*) const override;

            QPointF calcNearestPoint(const QPointF& pos) const override;

            double pointToPosValue(const QPointF& pos) const override;
            QPointF posValueToPoint(double val) const override;

        private:
            // If this line is represented as line
            // in the Poincare disk, then
            //  * isLine is set to true;
            //  * a, b are set to intersection points of this line
            //    and absolute.
            // 
            // If this line is represented as circle
            // in the Poincare disk, then
            //  * isLine is set to false;
            //  * o is set to the center of the circle;
            //  * r is set to the radius of the circle.
            void getEuclidian(
                bool& isLine,

                // For line
                Point& a,
                Point& b,

                // For circle
                Point& o,
                double& r
            ) const;

            Point p1;
            Point p2;

            static constexpr double paintWidth = 3e-3;
    };
}
