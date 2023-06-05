#include "Line.h"

#include "Point.h"

#include "core/AbstractTransformation.h"

#include "util/math.h"
#include "util/cramer.h"

#include <cassert>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <complex>

using namespace std::complex_literals;

namespace hyperbolic::impl {
    Line::Line()
      : Line(Point(), Point())
   {}

    Line::Line(const Point& p1, const Point& p2)
      : p1(p1),
        p2(p2)
   {}

    Object* Line::clone() const {
        return new Line(*this);
    }

    void Line::paint(QPainter* qp, double scale, const QColor& color) const {
        toPoincare().paint(qp, scale, color);
    }

    QRectF Line::boundingRect(double scale) const {
        return toPoincare().boundingRect(scale);
    }

    QPainterPath Line::shape(double scale) const {
        return toPoincare().shape(scale);
    }

    void Line::transform(const AbstractTransformation* t) {
        p1.transform(t);
        p2.transform(t);
    }

    AbstractPoint* Line::calcNearestPoint(const AbstractPoint* pt) const {
        auto p = *static_cast<const Point*>(pt);
        auto ans = intersect(*this, perp(*this, p));
        assert(ans.has_value());
        return new Point(*ans);
    }

    double Line::pointToPosValue(const AbstractPoint* pt) const {
        auto p = *static_cast<const Point*>(pt);

        auto ep1 = EPoint(p1.getPos());
        auto ep2 = EPoint(p2.getPos());
        auto ep = EPoint(p.getPos());

        return sgn(dot(ep2 - ep1, ep - ep1)) * dist(p1, p) / dist(p1, p2);
    }

    AbstractPoint* Line::posValueToPoint(double val) const {
        auto ep1 = EPoint(p1.getPos());
        auto ep2 = EPoint(p2.getPos());

        auto [A, B, C] = getABC();
        auto [p1_, p2_] = getIntersectionsWithAbsolute(A, B, C);
        auto ea = EPoint(p1_.getPos());
        auto eb = EPoint(p2_.getPos());
        if (euclidian::impl::dist(ea, ep1) > euclidian::impl::dist(ea, ep2)) {
            std::swap(ea, eb);
        }

        auto a = euclidian::impl::dist(ea, ep1);
        auto b = euclidian::impl::dist(eb, ep1);
        auto r = val * dist(p1, p2);
        auto m = exp(2 * r);
        auto x = (m - 1) * a * b / (m * a + b);

        auto ans = ep1 + euclidian::impl::norm(ep2 - ep1) * x;

        return new Point(ans.getPos());
    }

    EArc Line::toPoincare() const {
        auto [a, b, c] = getABC();
        auto [p1_, p2_] = getIntersectionsWithAbsolute(a, b, c);

        auto ansP1 = EPoint(p1_.getPos());
        auto ansP2 = EPoint(p2_.getPos());

        auto ep1 = p1.toPoincare();
        auto ep2 = p2.toPoincare();

        auto p = ep1.x + 1i * ep1.y;
        auto q = ep2.x + 1i * ep2.y;

        auto o_ = 
            (p * (q * std::conj(q) + 1.) - q * (p * std::conj(p) + 1.)) /
        // ---------------------------------------------------------------
                    (p * std::conj(q) - q * std::conj(p));

        auto o = EPoint(std::real(o_), std::imag(o_));

        return EArc(o, ansP1, ansP2);
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

        auto t = sq(a) + sq(b);
        a /= t;
        b /= t;
        c /= t;

        return {a, b, c};
    }

    std::pair<Point, Point> getIntersectionsWithAbsolute(double a, double b, double c) {
        Point H1, H2;

        if (eq(b, 0)) {
            auto x = H1.x = H2.x = -c / a;
            H1.y = sqrt(1 - sq(x));
            H2.y = -H1.y;
        } else {
            int n;
            solveSqEq(
                sq(a) + sq(b),
                2 * a * c,
                sq(c) - sq(b),
                n,
                H1.x,
                H2.x
            );
            assert(n == 2);

            H1.y = -(a * H1.x + c) / b;
            H2.y = -(a * H2.x + c) / b;
        }

        return {H1, H2};
    }

    std::pair<Point, Point> getTwoPointsOnLine(double a, double b, double c) {
        // H1 and H2 are intersections of line and absolute
        // OM is perpendicular from O(0, 0) on line
        // Ans points are midpoins of M and Hs

        auto M = Point(a, b) * (-c / (sq(a) + sq(b)));
        auto [H1, H2] = getIntersectionsWithAbsolute(a, b, c);

        return {
            (H1 + M) / 2,
            (H2 + M) / 2
        };
    }

    Line perp(const Line& l, const Point& p) {
        // Two lines with equations
        //     Ax + By + C = 0
        // and
        //     Dx + Ey + F = 0
        // are perpendicular when
        //     AD + BE = CF

        auto x = p.x;
        auto y = p.y;
        auto [a, b, c] = l.getABC();

        auto d = c * y + b;
        auto e = -(c * x + a);
        auto f = -(d * x + e * y);

        auto [p1, p2] = getTwoPointsOnLine(d, e, f);

        return Line(p1, p2);
    }

    Line perp(const Point& p, const Line& l) {
        return perp(l, p);
    }

    std::optional<Point> intersect(const Line& l1, const Line& l2) {
        auto [a1, b1, c1] = l1.getABC();
        auto [a2, b2, c2] = l2.getABC();

        auto cramerAns = cramer({
            {a1, b1, -c1},
            {a2, b2, -c2},
        });

        if (cramerAns.isEmpty()) return std::nullopt;

        auto pt = Point(cramerAns[0], cramerAns[1]);

        if (geq(sq(pt.x) + sq(pt.y), 1)) return std::nullopt;

        return pt;
    }
}
