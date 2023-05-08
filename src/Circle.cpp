#include "Circle.h"

#include "Transformation.h"

#include <QRectF>
#include <QPainterPath>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <memory>

Circle::Circle() : Circle(Point(), 1) {}
Circle::Circle(const Point& o, double r) : GeometryObject(), o(o), r(r) {}

Object* Circle::clone() const {
    return new Circle(*this);
}

void Circle::paint(QPainter* qp) const {
    auto pen = qp->pen();
    pen.setWidthF(paintWidth);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    qp->setPen(pen);

    qp->drawEllipse(boundingRect());
}

QRectF Circle::boundingRect() const {
    return QRectF(
        o.x - r,
        o.y - r,
        r * 2,
        r * 2
    );
}

QPainterPath Circle::shape() const {
    QPainterPath path;
    path.addEllipse(QRectF(
        o.x - r - paintWidth,
        o.y - r - paintWidth,
        (paintWidth + r) * 2,
        (paintWidth + r) * 2
    ));
    path.addEllipse(QRectF(
        o.x - r + paintWidth,
        o.y - r + paintWidth,
        (r - paintWidth) * 2,
        (r - paintWidth) * 2
    ));
    return path;
}

GeometryObject* Circle::transformed(const Transformation& t) const {
    return new Circle(
        *std::unique_ptr<Point>(static_cast<Point*>(o.transformed(t))),
        r * t.getScale()
    );
}

QPointF Circle::calcNearestPoint(const QPointF& pos) const {
    auto p = Point(pos);
    auto op = p - o;
    return (o + norm(op) * r).getPos();
}
