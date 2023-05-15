#include "Line.h"

#include "math.h"
#include "Point.h"
#include "Transformation.h"

#include <QRectF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <math.h>
#include <QPointF>
#include <memory>

Line::Line() {}

Line::Line(const QPointF& p1, const QPointF& p2)
  : p1(p1),
    p2(p2)
{}

Line::Line(const Point& p1, const Point& p2)
  : Line(p1.getPos(), p2.getPos())
{}

Object* Line::clone() const {
    return new Line(*this);
}

void Line::paint(QPainter* qp, const QColor& color) const {
    auto pen = qp->pen();
    pen.setWidthF(paintWidth);
    pen.setColor(color);
    qp->setPen(pen);

    auto [p1, p2] = getTwoBoundingPoints();
    qp->drawLine(p1, p2);
}

QRectF Line::boundingRect() const {
    auto [p1, p2] = getTwoBoundingPoints();
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
    auto [p1, p2] = getTwoBoundingPoints();

    QPainterPath path;
    path.moveTo(p1 - d);
    path.lineTo(p1 + d);
    path.lineTo(p2 + d);
    path.moveTo(p2 - d);
    return path;
}

GeometryObject* Line::transformed(const Transformation& t) const {
    return new Line(
        t.transform(p1),
        t.transform(p2)
    );
}

QPointF Line::calcNearestPoint(const QPointF& p) const {
    auto n = getNorm() * getDist(p);
    auto ans = p + n;
    if (!eq(getDist(ans), 0)) {
        ans = p - n;
    }
    return ans;
}

double Line::pointToPosValue(const QPointF& p) const {
    auto p1p = p - p1;
    auto p1p2 = p2 - p1;
    return QPointF::dotProduct(p1p, getDir()) / len(p1p2);
}

QPointF Line::posValueToPoint(double val) const {
    auto p1p2 = p2 - p1;
    return p1 + val * len(p1p2) * getDir();
}

std::tuple<double, double, double> Line::getABC() const {
    double a, b, c;
    if (eq(p1.x(), p2.x())) {
        a = 1;
        b = 0;
        c = -p1.x();
    } else {
        a = p2.y() - p1.y();
        b = p1.x() - p2.x();
        c = -(a * p1.x() + b * p1.y());
    }
    return {a, b, c};
}

std::pair<Point, Point> Line::getTwoPoints() const {
    return {Point(p1), Point(p2)};
}

std::pair<QPointF, QPointF> Line::getTwoBoundingPoints() const {
    auto [a, b, c] = getABC();
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

QPointF Line::getDir() const {
    auto res = p2 - p1;
    res /= len(res);
    return res;
}

QPointF Line::getNorm() const {
    auto dir = getDir();
    return QPointF(dir.y(), -dir.x());
}

double Line::getDist(const QPointF& p) const {
    auto [a, b, c] = getABC();
    return abs(a * p.x() + b * p.y() + c) / hypot(a, b);
}

double dist(const Line& l, const Point& p) {
    return l.getDist(p.getPos());
}

double dist(const Point& p, const Line& l) {
    return l.getDist(p.getPos());
}

Point norm(const Line& l) {
    return Point(l.getNorm());
}

Point dir(const Line& l) {
    return Point(l.getDir());
}
