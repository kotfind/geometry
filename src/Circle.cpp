#include "Circle.h"

#include "exception.h"

#include <QRectF>
#include <QPainterPath>

Circle::Circle() : Circle(Point(), 1) {}
Circle::Circle(const Point& o, double r) : GeometryObject(), o(o), r(r) {}

void Circle::paint(QPainter* painter) const {
    throw NotImplementedError();
}

QRectF Circle::boundingRect() const {
    throw NotImplementedError();
}

QPainterPath Circle::shape() const {
    throw NotImplementedError();
}
