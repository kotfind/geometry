#pragma once

#include "AbstractPoint.h"

class QPointF;
class QJsonObject;

class Point : public AbstractPoint {
    public:
        Point();
        Point(double x, double y);
        explicit Point(const QPointF& pos);

        enum { Type = 1 << 1 };
        int getType() const override { return Type; }

        Object* clone() const override;

        void paint(QPainter* painter, const QColor& color) const override;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject& json) override;

        GeometryObject* transformed(const Transformation*) const override;
        Point* untransformed(const Transformation*) const;

        QPointF calcNearestPoint(const QPointF& pos) const override;

        double pointToPosValue(const QPointF& pos) const override;
        QPointF posValueToPoint(double val) const override;

        void setPos(const QPointF& pos) override;

        QPointF getPos() const override;

        double x, y;

    private:
        static constexpr double paintRadius = 5e-3;
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

double dist(const Point& p1, const Point& p2);
double len(const Point& v);

Point rot(const Point& v, double sin_a, double cos_a);
Point rot(const Point& v, double a);

Point norm(const Point& v);

Point perp(const Point& v);
