#include "Circle.h"

#include <QRectF>
#include <QPainterPath>
#include <QPainter>
#include <QPen>
#include <QDebug>

Circle::Circle() : Circle(Point(), 1) {}
Circle::Circle(const Point& o, double r) : GeometryObject(), o(o), r(r) {}

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
    path.addEllipse(boundingRect());
    return path;
}
