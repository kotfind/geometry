#include "Circle.h"

#include "core/AbstractTransformation.h"

#include "util/math.h"

#include <QRectF>
#include <QPainterPath>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <memory>
#include <math.h>

namespace euclidian::impl {
    Circle::Circle()
      : Circle(Point(), 1)
    {}

    Circle::Circle(const Point& o, double r)
      : o(o),
        r(r)
    {}

    Object* Circle::clone() const {
        return new Circle(*this);
    }

    void Circle::paint(QPainter* qp, const QColor& color) const {
        auto pen = qp->pen();
        pen.setWidthF(paintWidth);
        pen.setStyle(Qt::SolidLine);
        pen.setColor(color);
        qp->setPen(pen);

        qp->drawEllipse(getCircleRect(o.getPos(), r));
    }

    QRectF Circle::boundingRect() const {
        return getCircleRect(o.getPos(), r + paintWidth);
    }

    QPainterPath Circle::shape() const {
        QPainterPath path;
        path.addEllipse(getCircleRect(o.getPos(), r + paintWidth));
        path.addEllipse(getCircleRect(o.getPos(), r - paintWidth));
        return path;
    }

    void Circle::transform(const AbstractTransformation* t) {
        auto p = o + Point(r, 0);

        o.transform(t);
        p.transform(t);

        r = dist(o, p);
    }

    AbstractPoint* Circle::calcNearestPoint(const AbstractPoint* pt) const {
        auto p = *std::unique_ptr<Point>(
            static_cast<Point*>(pt->clone())
        );
        auto op = p - o;
        return new Point(o + norm(op) * r);
    }

    double Circle::pointToPosValue(const AbstractPoint* pt) const {
        auto p = *static_cast<const Point*>(pt);
        auto op = p - o;
        return atan2(op.y, op.x);
    }

    AbstractPoint* Circle::posValueToPoint(double val) const {
        return new Point(o + r * Point(cos(val), sin(val)));
    }

    std::tuple<Point, Point, Point> Circle::getThreePoints() const {
        Point pts[3];
        for (int i = 0; i < 3; ++i) {
            auto a = 2 * M_PI / 3 * i;
            pts[i] = o + r * Point(cos(a), sin(a));
        }
        return { pts[0], pts[1], pts[2] };
    }

    void intersect(
        const Circle& w1,
        const Circle& w2,
        int& n,
        Point& p1,
        Point& p2
    ) {
        //    /\
        // r1/   \ r2
        //  /a     \
        //  ---------
        //      d

        auto r1 = w1.r;
        auto r2 = w2.r;
        auto o1 = w1.o;
        auto o2 = w2.o;
        auto d = dist(o1, o2);

        if (gr(abs(r1 - r2), d) || gr(d, r1 + r2)) {
            n = 0;
            return;
        }

        if (eq(abs(r1 - r2), d) || eq(d, r1 + r2)) {
            n = 1;
            p1 = Point(norm(o2 - o1) * r1 + o1);
            return;
        }

        auto cos_a = (sq(r1) + sq(d) - sq(r2)) /
                  // ---------------------
                         (2 * r1 * d);

        auto sin_a = sqrt(1 - cos_a*cos_a);

        auto v = norm(o2 - o1) * r1;

        n = 2;
        p1 = Point(rot(v, +sin_a, cos_a) + o1);
        p2 = Point(rot(v, -sin_a, cos_a) + o1);
    }

    void intersect(
        const Line& l,
        const Circle& w,
        int& n,
        Point& p1,
        Point& p2
    ) {
        auto o = w.o;
        auto r = w.r;

        double d = dist(o, l);
        if (gr(d, r)) {
            n = 0;
            return;
        }

        auto h = o + norm(l) * d;
        if (!eq(dist(h, l), 0)) {
            h = o - norm(l) * d;
        }
        if (eq(d, r)) {
            n = 1;
            p1 = h;
            return;
        }

        auto x = sqrt(sq(r) - sq(d)) * dir(l);

        n = 2;
        p1 = Point(h + x);
        p2 = Point(h - x);
    }

    void intersect(
        const Circle& w,
        const Line& l,
        int& n,
        Point& p1,
        Point& p2
    ) {
        intersect(l, w, n, p1, p2);
    }
}
