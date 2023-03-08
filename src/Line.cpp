#include "Line.h"

#include "exception.h"

#include <QRectF>

Line::Line() : Line(0, 1, 0) {}
Line::Line(double a, double b, double c) : GeometryObject(), a(a), b(b), c(c) {}

void Line::paint(QPainter* painter) const {
    throw NotImplementedError();
}

QRectF Line::boundingRect() const {
    throw NotImplementedError();
}
