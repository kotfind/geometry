#include "Circle.h"

#include "Transformation.h"

#include <QRectF>
#include <QPainterPath>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <memory>

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

void Circle::paint(QPainter* qp) const {
    auto pen = qp->pen();
    pen.setWidthF(paintWidth);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
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

GeometryObject* Circle::transformed(const Transformation& t) const {
    return new Circle(
        t.transform(o),
        r * t.getScale()
    );
}

QPointF Circle::calcNearestPoint(const QPointF& pos) const {
    auto p = Point(pos);
    auto op = p - o;
    return (o + norm(op) * r).getPos();
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
