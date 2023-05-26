#pragma once

#include "core/AbstractPoint.h"

#include "euclidian/Point.h"

#include <QPointF>

namespace hyperbolic::impl {
    class Point : public AbstractPoint {
        public:
            Point();
            Point(double x, double y);
            // pos is in euclidian
            explicit Point(const QPointF& pos);

            enum { Type = 1 << 1 };
            int getType() const override { return Type; }

            Object* clone() const override;

            void paint(QPainter* painter, const QColor& color) const override;
            QRectF boundingRect() const override;
            QPainterPath shape() const override;

            QJsonObject toJson() const override;
            void fromJson(const QJsonObject& json) override;

            QPointF getPos() const override;
            void setPos(const QPointF& pos) override;

            // Returns point representaion on Poincare disk
            euclidian::impl::Point getEuclidian() const;

            // In beltrami coordinates
            double x, y;
    };

    bool operator==(const Point& p1, const Point& p2);

    Point& operator+=(Point& lhs, const Point& rhs);
    Point& operator-=(Point& lhs, const Point& rhs);
    Point& operator*=(Point& lhs, double rhs);
    Point& operator/=(Point& lhs, double rhs);

    Point operator+(const Point& lhs, const Point& rhs);
    Point operator-(const Point& lhs, const Point& rhs);
    Point operator*(const Point& lhs, double rhs);
    Point operator*(double lhs, const Point& rhs);
    Point operator/(const Point& lhs, double rhs);
}
