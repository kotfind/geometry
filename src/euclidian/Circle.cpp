#include "Circle.h"

#include "core/Transformation.h"

#include "util/math.h"

#include <QRectF>
#include <QPainterPath>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <memory>
#include <math.h>

Circle::Circle() : Circle(Point(), 1) {}
Circle::Circle(const QPointF& o, double r)
  : o(o),
    r(r)
{}
Circle::Circle(const Point& o, double r)
  : Circle(o.getPos(), r)
{}

Object* Circle::clone() const {
    return new Circle(*this);
}

void Circle::paint(QPainter* qp, const QColor& color) const {
    auto pen = qp->pen();
    pen.setWidthF(paintWidth);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(color);
    qp->setPen(pen);

    qp->drawEllipse(getRect(o, r));
}

QRectF Circle::boundingRect() const {
    return getRect(o, r + paintWidth);
}

QPainterPath Circle::shape() const {
    QPainterPath path;
    path.addEllipse(getRect(o, r + paintWidth));
    path.addEllipse(getRect(o, r - paintWidth));
    return path;
}

GeometryObject* Circle::transformed(const Transformation* t) const {
    auto p = o + QPointF(r, 0);

    auto o_ = t->transform(o);
    auto p_ = t->transform(p);
    return new Circle(
        o_,
        dist(o_, p_)
    );
}

QPointF Circle::calcNearestPoint(const QPointF& pos) const {
    auto p = Point(pos);
    auto o_ = Point(o);
    auto op = p - o_;
    return (o_ + norm(op) * r).getPos();
}

double Circle::pointToPosValue(const QPointF& p) const {
    auto op = p - o;
    return atan2(op.y(), op.x());
}

QPointF Circle::posValueToPoint(double val) const {
    return o + r * QPointF(cos(val), sin(val));
}

Point Circle::getO() const {
    return Point(o);
}

double Circle::getR() const {
    return r;
}

QRectF Circle::getRect(const QPointF& center, double radius) {
    return QRectF(
        center.x() - radius,
        center.y() - radius,
        radius * 2,
        radius * 2
    );
}
