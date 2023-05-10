#include "Segment.h"

#include "math.h"
#include "Transformation.h"

#include <QPen>
#include <math.h>
#include <QPainterPath>

Segment::Segment() : Segment(QPointF(), QPointF())
{}

Segment::Segment(const QPointF& p1, const QPointF& p2)
  : p1(p1),
    p2(p2)
{}

Object* Segment::clone() const {
    return new Segment(*this);
}

std::pair<QPointF, QPointF> Segment::getTwoQPointFs() const {
    return {p1, p2};
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

void Segment::paint(QPainter* qp) const {
    auto pen = qp->pen();
    pen.setWidthF(paintWidth);
    qp->setPen(pen);

    auto [p1, p2] = getTwoQPointFs();
    qp->drawLine(p1, p2);
}

QRectF Segment::boundingRect() const {
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

QPainterPath Segment::shape() const {
    auto d = getNorm() * paintWidth;
    auto [p1, p2] = getTwoQPointFs();

    QPainterPath path;
    path.moveTo(p1 - d);
    path.lineTo(p1 + d);
    path.lineTo(p2 + d);
    path.moveTo(p2 - d);
    return path;
}

GeometryObject* Segment::transformed(const Transformation& t) const {
    auto [p1, p2] = getTwoQPointFs();
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

QPointF Segment::getNorm() const {
    auto [a, b, c] = getABC();
    QPointF res(a, b);
    res /= std::hypot(res.x(), res.y());
    return res;
}
