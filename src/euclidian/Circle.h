#pragma once

#include "Point.h"

#include "core/GeometryObject.h"

namespace euclidian::impl {
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

            // posValue for Circle is angle
            double pointToPosValue(const AbstractPoint* pos) const override;
            AbstractPoint* posValueToPoint(double val) const override;

            Point o;
            double r;
            
        private:
            static constexpr double paintWidth = 3e-3;
    };
}
