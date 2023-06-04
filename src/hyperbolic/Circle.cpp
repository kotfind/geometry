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
        auto [p1, p2, p3] = getThreePoints();
        p1.transform(t);
        p2.transform(t);
        p3.transform(t);
        fromThreePoints(p1, p2, p3);
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

    void Circle::fromThreePoints(const Point& p1, const Point& p2, const Point& p3) {
        auto ans = cramer({
            {p1.x, p1.y, 1, sqrt(1 - sq(p1.x) - sq(p1.y))},
            {p2.x, p2.y, 1, sqrt(1 - sq(p2.x) - sq(p2.y))},
            {p3.x, p3.y, 1, sqrt(1 - sq(p3.x) - sq(p3.y))},
        });
        assert(!ans.isEmpty());

        fromABC(ans[0], ans[1], ans[2]);
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

    void Circle::fromABC(double a, double b, double c) {
        // Find two points that are both on the circle
        // and on the radius that goes through the center

        // Vector (a, b) goes through center of circle

        auto v = Point(a, b) / hypot(a, b);

        // Find r knowing that
        //     (v * r) \in (sqrt(1 - x^2 - y^2) = ax + by + c)

        int n;
        double r1;
        double r2;
        solveSqEq(
            sq(a) + sq(b) + 1,
            2 * c * sqrt(sq(a) + sq(b)),
            sq(c) - 1,
            n,
            r1,
            r2
        );
        assert(n == 2);

        auto A = v * r1;
        auto B = v * r2;

        o = midpoint(A, B);
        r = dist(A, o);
    }

    void intersect(
        const Line& l,
        const Circle& w,
        int& n,
        Point& p1,
        Point& p2
    ) {
        auto [a, b, c] = l.getABC();
        auto [d, e, f] = w.getABC();

        if (eq(b, 0)) {
            auto x = -c / a;
            int N;
            double y1;
            double y2;

            solveSqEq(
                sq(a) * (sq(e) + 1),
                2 * a * e * (a * f - d * c),
                sq(a * f - d * c) - sq(a) + sq(c),
                N,
                y1,
                y2
            );

            n = N;
            p1 = Point(x, y1);
            p2 = Point(x, y2);
        } else {
            int N;
            double x1;
            double x2;
            solveSqEq(
                sq(b * d - a * e) + sq(a) + sq(b),
                2 * (a * c + (b * d - a * e) * (b * f - e * c)),
                sq(b * f - e * c) - sq(b) + sq(c),
                N,
                x1,
                x2
            );

            auto y1 = -(a * x1 + c) / b;
            auto y2 = -(a * x2 + c) / b;

            n = N;
            p1 = Point(x1, y1);
            p2 = Point(x2, y2);
        }
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

    void intersect(
        const Circle& w1,
        const Circle& w2,
        int& n,
        Point& p1,
        Point& p2
    ) {
        auto [a, b, c] = w1.getABC();
        auto [d, e, f] = w2.getABC();

        auto [lp1, lp2] = getTwoPointsOnLine(
            d - a,
            e - b,
            f - c
        );

        auto l = Line(lp1, lp2);

        intersect(w1, l, n, p1, p2);
    }
}
