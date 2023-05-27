#include "Point.h"

#include "core/AbstractTransformation.h"

#include "util/math.h"
#include "util/getOrThrow.h"

#include "euclidian/Point.h"

#include <QRectF>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QJsonObject>

using EPoint = euclidian::impl::Point;

namespace hyperbolic::impl {
    Point::Point()
      : Point(0, 0)
    {}

    Point::Point(double x, double y)
      : x(x),
        y(y)
    {}

    Point::Point(const EPoint& p)
    {
        fromPoincare(p);
    }

    Object* Point::clone() const {
        return new Point(*this);
    }

    void Point::paint(QPainter* qp, const QColor& color) const {
        toPoincare().paint(qp, color);
    }

    QRectF Point::boundingRect() const {
        return toPoincare().boundingRect();
    }

    QPainterPath Point::shape() const {
        return toPoincare().shape();
    }

    QJsonObject Point::toJson() const {
        QJsonObject json;
        json["x"] = x;
        json["y"] = y;
        return json;
    }

    void Point::fromJson(const QJsonObject& json) {
        x = getOrThrow(json["x"]).toDouble();
        y = getOrThrow(json["y"]).toDouble();
    }

    void Point::setPos(const QPointF& pos) {
        x = pos.x();
        y = pos.y();
    }

    QPointF Point::getPos() const {
        return QPointF(x, y);
    }

    EPoint Point::toPoincare() const {
        return EPoint(x, y) / (1 + sqrt(1 - sq(x) - sq(y)));
    }

    void Point::fromPoincare(const EPoint& p) {
        auto a = p.x;
        auto b = p.y;
        auto t = 1 + sq(a) + sq(b);
        x = 2 * a / t;
        y = 2 * b / t;
    }

    bool operator==(const Point& p1, const Point& p2) {
        // TODO: Use hyperbolic metrics
        return eq(p1.x, p2.x) && eq(p1.y, p2.y);
    }

    Point& operator+=(Point& lhs, const Point& rhs) {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    Point& operator-=(Point& lhs, const Point& rhs){
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    Point& operator*=(Point& lhs, double rhs){
        lhs.x *= rhs;
        lhs.y *= rhs;
        return lhs;
    }

    Point& operator/=(Point& lhs, double rhs){
        lhs.x /= rhs;
        lhs.y /= rhs;
        return lhs;
    }

    Point operator+(const Point& lhs, const Point& rhs){
        auto ans = lhs;
        ans += rhs;
        return ans;
    }

    Point operator-(const Point& lhs, const Point& rhs){
        auto ans = lhs;
        ans -= rhs;
        return ans;
    }

    Point operator*(const Point& lhs, double rhs) {
        auto ans = lhs;
        ans *= rhs;
        return ans;
    }

    Point operator*(double lhs, const Point& rhs) {
        return rhs * lhs;
    }

    Point operator/(const Point& lhs, double rhs) {
        auto ans = lhs;
        ans /= rhs;
        return ans;
    }
}
