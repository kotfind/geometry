#include "Circle.h"

#include "core/AbstractTransformation.h"

#include "util/math.h"
#include "util/cramer.h"

#include <QRectF>
#include <QPainterPath>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <memory>
#include <math.h>

namespace hyperbolic::impl {
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
        toPoincare().paint(qp, color);
    }

    QRectF Circle::boundingRect() const {
        return toPoincare().boundingRect();
    }

    QPainterPath Circle::shape() const {
        return toPoincare().shape();
    }

    void Circle::transform(const AbstractTransformation* t) {
        // FIXME
        // auto p = o + Point(r, 0);

        // o.transform(t);
        // p.transform(t);

        // r = dist(o, p);
    }

    AbstractPoint* Circle::calcNearestPoint(const AbstractPoint* pt) const {
        // FIXME
        return new Point(0, 0);
    }

    double Circle::pointToPosValue(const AbstractPoint* pt) const {
        // FIXME
        return 0;
    }

    AbstractPoint* Circle::posValueToPoint(double val) const {
        // FIXME
        return new Point(0, 0);
    }

    ECircle Circle::toPoincare() const {
        auto C = EPoint(o.getPos());
        auto c = len(C);
        auto dir = norm(C);

        auto m = ((1 + c) / (1 - c)) * exp(-2 * r);
        auto a = (m - 1) / (m + 1);
        auto A = Point((dir * a).getPos()).toPoincare();

        auto n = ((1 + c) / (1 - c)) * exp(2 * r);
        auto b = (n - 1) / (n + 1);
        auto B = Point((dir * b).getPos()).toPoincare();

        auto O = (A + B) / 2;
        auto R = dist(O, A);

        return ECircle(O, R);
    }

    std::tuple<Point, Point, Point> Circle::getThreePoints() const {
        auto ew = toPoincare();
        auto [ep1, ep2, ep3] = ew.getThreePoints();
        return {
            Point(ep1),
            Point(ep2),
            Point(ep3)
        };
    }

    std::tuple<double, double, double> Circle::getABC() const {
        auto [p1, p2, p3] = getThreePoints();
        auto ans = cramer({
            {p1.x, p1.y, 1, sqrt(1 - sq(p1.x) - sq(p1.y))},
            {p2.x, p2.y, 1, sqrt(1 - sq(p2.x) - sq(p2.y))},
            {p3.x, p3.y, 1, sqrt(1 - sq(p3.x) - sq(p3.y))},
        });

        assert(!ans.isEmpty());

        return {
            ans[0],
            ans[1],
            ans[2]
        };
    }
}
