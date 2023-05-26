#include "Point.h"

#include "core/AbstractTransformation.h"

#include "util/math.h"
#include "util/getOrThrow.h"

#include <QRectF>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QJsonObject>

namespace euclidian::impl {
    Point::Point()
      : Point(0, 0)
    {}

    Point::Point(double x, double y)
      : x(x),
        y(y)
    {}

    Point::Point(const QPointF& pos)
      : Point(pos.x(), pos.y())
    {}

    Object* Point::clone() const {
        return new Point(*this);
    }

    void Point::paint(QPainter* qp, const QColor& color) const {
        auto pen = qp->pen();
        pen.setWidth(0);
        pen.setColor(color);
        qp->setPen(pen);

        auto brush = qp->brush();
        brush.setColor(color);
        brush.setStyle(Qt::SolidPattern);
        qp->setBrush(brush);

        qp->drawEllipse(boundingRect());
    }

    QRectF Point::boundingRect() const {
        return QRectF(
            x - paintRadius,
            y - paintRadius,
            paintRadius * 2,
            paintRadius * 2
        );
    }

    QPainterPath Point::shape() const {
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
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

    bool operator==(const Point& p1, const Point& p2) {
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

    double dot(const Point& lhs, const Point& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    double dist(const Point& p1, const Point& p2) {
        return len(p1 - p2);
    }

    double len(const Point& v) {
        return len(v.getPos());
    }

    Point rot(const Point& v, double sin_a, double cos_a) {
        return Point(
            v.x*cos_a - v.y*sin_a,
            v.y*cos_a + v.x*sin_a
        );
    }

    Point rot(const Point& v, double a) {
        return rot(v, sin(a), cos(a));
    }

    Point norm(const Point& v) {
        return v / len(v);
    }

    Point perp(const Point& v) {
        return {-v.y, v.x};
    }

    bool collinear(const Point& a, const Point& b, const Point& c, double epsilon) {
        return abs((b.y - a.y) * (c.x - b.x) - (c.y - b.y) * (b.x - a.x)) < epsilon;
    }
}
