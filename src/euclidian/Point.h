#pragma once

#include "core/AbstractPoint.h"

#include "util/math.h"

class QPointF;
class QJsonObject;

namespace euclidian::impl {
    class Point : public AbstractPoint {
        public:
            Point();
            Point(double x, double y);
            explicit Point(const QPointF& pos);

            enum { Type = 1 << 1 };
            int getType() const override { return Type; }

            Object* clone() const override;

            void paint(QPainter* painter, double scale, const QColor& color) const override;
            QRectF boundingRect(double scale) const override;
            QPainterPath shape(double scale) const override;

            QJsonObject toJson() const override;
            void fromJson(const QJsonObject& json) override;

            void setPos(const QPointF& pos) override;

            QPointF getPos() const override;

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

    double dot(const Point& lhs, const Point& rhs);
    double cross(const Point& lhs, const Point& rhs);

    double dist(const Point& p1, const Point& p2);
    double len(const Point& v);

    Point rot(const Point& v, double sin_a, double cos_a);
    Point rot(const Point& v, double a);

    Point norm(const Point& v);

    Point perp(const Point& v);

    bool collinear(
        const Point& a,
        const Point& b,
        const Point& c
    );
}
