#include "Line.h"

#include "Point.h"
#include "config.h"

#include "core/AbstractTransformation.h"

#include "util/math.h"
#include "util/cramer.h"

#include <QRectF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <math.h>
#include <QPointF>
#include <memory>

namespace euclidian::impl {
    Line::Line() {}

    Line::Line(const Point& p1, const Point& p2)
      : p1(p1),
        p2(p2)
    {}

    Object* Line::clone() const {
        return new Line(*this);
    }

    void Line::paint(QPainter* qp, double scale, const QColor& color) const {
        auto pen = qp->pen();
        pen.setWidthF(scale * paintWidth);
        pen.setColor(color);
        qp->setPen(pen);

        auto [p1, p2] = getTwoBoundingPoints();
        qp->drawLine(p1.getPos(), p2.getPos());
    }

    QRectF Line::boundingRect(double scale) const {
        auto [p1, p2] = getTwoBoundingPoints();
        return QRectF(p1.getPos(), p2.getPos()).normalized();
    }

    QPainterPath Line::shape(double scale) const {
        auto d = norm(*this) * scale * paintWidth;
        auto [p1, p2] = getTwoBoundingPoints();

        QPainterPath path;
        path.moveTo((p1 - d).getPos());
        path.lineTo((p1 + d).getPos());
        path.lineTo((p2 + d).getPos());
        path.moveTo((p2 - d).getPos());
        return path;
    }

    void Line::transform(const AbstractTransformation* t) {
        p1.transform(t);
        p2.transform(t);
    }

    AbstractPoint* Line::calcNearestPoint(const AbstractPoint* pt) const {
        auto p = *static_cast<const Point*>(pt);
        auto n = norm(*this) * dist(*this, p);
        auto ans = p + n;
        if (!eq(dist(*this, ans), 0)) {
            ans = p - n;
        }
        return new Point(ans);
    }

    double Line::pointToPosValue(const AbstractPoint* pt) const {
        auto p = *static_cast<const Point*>(pt);
        auto p1p = p - p1;
        auto p1p2 = p2 - p1;
        return dot(p1p, dir(*this)) / len(p1p2);
    }

    AbstractPoint* Line::posValueToPoint(double val) const {
        auto p1p2 = p2 - p1;
        return new Point(
            p1 + val * len(p1p2) * dir(*this)
        );
    }

    std::tuple<double, double, double> Line::getABC() const {
        double a, b, c;
        if (eq(p1.x, p2.x)) {
            a = 1;
            b = 0;
            c = -p1.x;
        } else {
            a = p2.y - p1.y;
            b = p1.x - p2.x;
            c = -(a * p1.x + b * p1.y);
        }
        return {a, b, c};
    }

    std::pair<Point, Point> Line::getTwoBoundingPoints() const {
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

    double dist(const Line& l, const Point& p) {
        auto [a, b, c] = l.getABC();
        return abs(a * p.x + b * p.y + c) / hypot(a, b);
    }

    double dist(const Point& p, const Line& l) {
        return dist(l, p);
    }

    Point norm(const Line& l) {
        auto d = dir(l);
        return Point(d.y, -d.x);
    }

    Point dir(const Line& l) {
        auto res = l.p2 - l.p1;
        res /= len(res);
        return res;
    }

    void intersect(const Line& l1, const Line& l2, int& n, Point& p) {
        auto [a1, b1, c1] = l1.getABC();
        auto [a2, b2, c2] = l2.getABC();

        auto ans = cramer({
            {a1, b1, -c1},
            {a2, b2, -c2},
        });

        if (ans.isEmpty()) {
            n = 0;
        } else {
            n = 1;
            p = Point(ans[0], ans[1]);
        }
    }
}
