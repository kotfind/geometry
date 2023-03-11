#include "Line.h"

#include "exception.h"
#include "global.h"

#include <QRectF>
#include <QPainter>
#include <QPen>
#include <math.h>

Line::Line() : Line(0, 1, 0) {}
Line::Line(double a, double b, double c) : GeometryObject(), a(a), b(b), c(c) {}

void Line::paint(QPainter* qp) const {
    auto pen = qp->pen();
    pen.setWidth(drawWidth);
    qp->setPen(pen);

    auto [p1, p2] = getTwoPoints();
    qp->drawLine(p1, p2);
}

QRectF Line::boundingRect() const {
    auto [p1, p2] = getTwoPoints();
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;
    return QRectF(
        std::min(x1, x2) - drawWidth,
        std::min(y1, y2) - drawWidth,
        abs(x1 - x2)     + drawWidth * 2,
        abs(y1 - y2)     + drawWidth * 2
    );
}

QPair<QPointF, QPointF> Line::getTwoPoints() const {
    if (eq(b, 0)) {
        double x = -c / a;
        return {{x, -1000}, {x, 1000}};
    } else {
        double x1 = -1000;
        double x2 = 1000;
        double y1 = -(a * x1 + c) / b;
        double y2 = -(a * x2 + c) / b;
        return {{x1, y1}, {x2, y2}};
    }
}
