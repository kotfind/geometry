#pragma once

#include "GeometryObject.h"
#include "Point.h"

class Circle : public GeometryObject {
    public:
        Type getType() const { return Type::Circle; }

        Circle();
        Circle(const QPointF& o, double r);
        Circle(const Point& o, double r);

        Object* clone() const override;

        void paint(QPainter* painter, const QColor& color) const override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        GeometryObject* transformed(const Transformation&) const override;

        QPointF calcNearestPoint(const QPointF& pos) const override;

        // posValue for Circle is angle
        double pointToPosValue(const QPointF& pos) const override;
        QPointF posValueToPoint(double val) const override;

        Point getO() const;
        double getR() const;
        
    private:
        static QRectF getRect(const QPointF& center, double radius);

        QPointF o;
        double r;

        static constexpr double paintWidth = 3e-3;
};
