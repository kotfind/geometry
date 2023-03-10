#include "Point.h"

#include "global.h"
#include "exception.h"

#include <QRectF>
#include <QPainter>
#include <QDebug>

Point::Point() : Point(0, 0) {}
Point::Point(double x, double y) : GeometryObject(), x(x), y(y) {}

void Point::paint(QPainter* qp, bool selected) const {
    auto color = selected ? Qt::red : Qt::black;

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

bool operator==(const Point& p1, const Point& p2) {
    return eq(p1.x, p2.x) && eq(p1.y, p2.y);
}
