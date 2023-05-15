#include "Segment.h"

#include "math.h"
#include "Transformation.h"
#include "Point.h"

#include <QPen>
#include <math.h>
#include <QPainterPath>

Segment::Segment() : Segment(QPointF(), QPointF())
{}

Segment::Segment(const QPointF& p1, const QPointF& p2)
  : p1(p1),
    p2(p2)
{}

Segment::Segment(const Point& p1, const Point& p2)
  : Segment(p1.getPos(), p2.getPos())
{}

Object* Segment::clone() const {
    return new Segment(*this);
}

void Segment::paint(QPainter* qp) const {
    auto pen = qp->pen();
    pen.setWidthF(paintWidth);
    qp->setPen(pen);

    auto [p1, p2] = getTwoPoints();
    qp->drawLine(p1, p2);
}

QRectF Segment::boundingRect() const {
    auto [p1, p2] = getTwoPoints();
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;
    return QRectF(
        std::min(x1, x2) - paintWidth,
        std::min(y1, y2) - paintWidth,
        abs(x1 - x2)     + paintWidth * 2,
        abs(y1 - y2)     + paintWidth * 2
    );
}

QPainterPath Segment::shape() const {
    auto d = getNorm() * paintWidth;
    auto [p1, p2] = getTwoPoints();

    QPainterPath path;
    path.moveTo(p1 - d);
    path.lineTo(p1 + d);
    path.lineTo(p2 + d);
    path.moveTo(p2 - d);
    return path;
}

GeometryObject* Segment::transformed(const Transformation& t) const {
    auto [p1, p2] = getTwoPoints();
    return new Segment(
        t.transform(p1),
        t.transform(p2)
    );
}

QPointF Segment::calcNearestPoint(const QPointF& p) const {
    // FIXME ? Segment --- not Line
    auto [a, b, c] = getABC();
    auto d = abs(a * p.x() + b * p.y() + c) / hypot(a, b); // TODO: use dist(Line, Point) func
    auto n = getNorm() * d;
    auto ans = p + n;
    if (!eq(a * ans.x() + b * ans.y() + c, 0)) {
        ans = p - n;
    }
    return ans;
}

std::pair<QPointF, QPointF> Segment::getTwoPoints() const {
    return {p1, p2};
}

std::pair<Point, Point> Segment::getTwoPoints_() const {
    return {Point(p1), Point(p2)};
}

std::tuple<double, double, double> Segment::getABC() const {
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

double Segment::getDist(const QPointF& p) const {
    auto [a, b, c] = getABC();
    return abs(a * p.x() + b * p.y() + c) / hypot(a, b);
}

QPointF Segment::getNorm() const {
    auto [a, b, c] = getABC();
    QPointF res(a, b);
    res /= std::hypot(res.x(), res.y());
    return res;
}

double dist(const Segment& s, const Point& p) {
    return s.getDist(p.getPos());
}

double dist(const Point& p, const Segment& s) {
    return dist(s, p);
}

Point norm(const Segment& s) {
    return Point(s.getNorm());
}

Point dir(const Segment& s) {
    auto ans = s.getNorm();
    return Point(-ans.y(), ans.x());
}
