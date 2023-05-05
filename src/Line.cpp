#include "Line.h"

#include "global.h"
#include "Point.h"
#include "Transformation.h"

#include <QRectF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <math.h>
#include <QPointF>
#include <memory>

Line::Line() : Line(0, 1, 0) {}
Line::Line(double a, double b, double c) : GeometryObject(), a(a), b(b), c(c) {}
Line::Line(const Point& p1, const Point& p2) : GeometryObject() {
    if (eq(p1.x, p2.x)) {
        a = 1;
        b = 0;
        c = -p1.x;
    } else {
        a = p2.y - p1.y;
        b = p1.x - p2.x;
        c = -(a * p1.x + b * p1.y);
    }
}

Object* Line::clone() const {
    return new Line(*this);
}

void Line::paint(QPainter* qp) const {
    auto pen = qp->pen();
    pen.setWidthF(paintWidth);
    qp->setPen(pen);

    auto [p1, p2] = getTwoQPointFs();
    qp->drawLine(p1, p2);
}

QRectF Line::boundingRect() const {
    auto [p1, p2] = getTwoQPointFs();
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;
    return QRectF(
        std::min(x1, x2) - paintWidth,
        std::min(y1, y2) - paintWidth,
        abs(x1 - x2)     + paintWidth * 2,
        abs(y1 - y2)     + paintWidth * 2
    );
}

QPainterPath Line::shape() const {
    auto d = getNorm() * paintWidth;
    auto [p1, p2] = getTwoQPointFs();

    QPainterPath path;
    path.moveTo(p1 - d);
    path.lineTo(p1 + d);
    path.lineTo(p2 + d);
    path.moveTo(p2 - d);
    return path;
}

QPair<Point, Point> Line::getTwoPoints() const {
    if (eq(b, 0)) {
        double x = -c / a;
        return {{x, -10}, {x, 10}};
    } else {
        double x1 = -10;
        double x2 = 10;
        double y1 = -(a * x1 + c) / b;
        double y2 = -(a * x2 + c) / b;
        return {{x1, y1}, {x2, y2}};
    }
}

QPair<QPointF, QPointF> Line::getTwoQPointFs() const {
    auto [p1, p2] = getTwoPoints();
    return {p1.getPos(), p2.getPos()};
}

QPointF Line::getNorm() const {
    QPointF res(a, b);
    res /= std::hypot(res.x(), res.y());
    return res;
}

GeometryObject* Line::transformed(const Transformation& t) const {
    auto [p1, p2] = getTwoPoints();
    return new Line(
        *std::unique_ptr<Point>(static_cast<Point*>(p1.transformed(t))),
        *std::unique_ptr<Point>(static_cast<Point*>(p2.transformed(t)))
    );
}

double dist(const Line& l, const Point& p) {
    return abs(l.a * p.x + l.b * p.y + l.c) / hypot(l.a, l.b);
}

double dist(const Point& p, const Line& l) {
    return dist(l, p);
}

Point norm(const Line& l) {
    return norm(Point(l.a, l.b));
}

Point dir(const Line& l) {
    auto ans = norm(l);
    return Point(-ans.y, ans.x);
}
