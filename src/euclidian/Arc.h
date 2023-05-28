#pragma once

#include "Point.h"

#include "core/GeometryObject.h"

#include <tuple>
#include <QPointF>

namespace euclidian::impl {
    class Point;

    // XXX: is not fully supported
    class Arc : public GeometryObject {
        public:
            Arc();
            Arc(const Point& o, const Point& p1, const Point& p2);

            enum { Type = 1 << 5 };
            int getType() const override { return Type; }

            Object* clone() const override;

            void paint(QPainter* painter, const QColor& color) const override;
            QRectF boundingRect() const override;
            QPainterPath shape() const override;

            void transform(const AbstractTransformation*) override;

            AbstractPoint* calcNearestPoint(const AbstractPoint* pos) const override;

            double pointToPosValue(const AbstractPoint* pt) const override;
            AbstractPoint* posValueToPoint(double val) const override;

            Point o;
            Point p1;
            Point p2;

        private:
            QPainterPath getPainterPath() const;

            static constexpr double paintWidth = 7e-3;
    };
}
