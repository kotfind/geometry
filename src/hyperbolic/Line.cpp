#include "Line.h"

#include "Point.h"

#include "core/AbstractTransformation.h"

#include "util/math.h"

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

    void Line::paint(QPainter* qp, const QColor& color) const {
        std::unique_ptr<GeometryObject>(getEuclidian())->paint(qp, color);
    }

    QRectF Line::boundingRect() const {
        return std::unique_ptr<GeometryObject>(getEuclidian())->boundingRect();
    }

    QPainterPath Line::shape() const {
        return std::unique_ptr<GeometryObject>(getEuclidian())->shape();
    }

    void Line::transform(const AbstractTransformation* t) {
        p1.transform(t);
        p2.transform(t);
    }

    AbstractPoint* Line::calcNearestPoint(const AbstractPoint* pos) const {
        // FIXME
        return new Point();
    }

    double Line::pointToPosValue(const AbstractPoint* pos) const {
        // FIXME
        return 0;
    }

    AbstractPoint* Line::posValueToPoint(double val) const {
        // FIXME
        return new Point();
    }

    GeometryObject* Line::getEuclidian() const {
        auto ep1 = p1.getEuclidian();
        auto ep2 = p2.getEuclidian();
        if (collinear(ep1, ep2, EPoint(0, 0), 0.01)) {
            // Line
            auto p = ep1;
            auto d = ep2 - ep1;

            // Solving equation system:
            // x^2 + y^2 = 1
            // x = p_x + t * d_x
            // y = p_y + t * d_y

            int n;
            double t1, t2;
            solveSqEq(
                sq(d.x) + sq(d.y), // a
                2 * (d.x * p.x + d.y * p.y),  // b
                sq(p.x) + sq(p.y) - 1, // c
                n,
                t1,
                t2
            );
            assert(n == 2);

            return new ELine(
                EPoint((p + t1 * d).getPos()),
                EPoint((p + t2 * d).getPos())
            );
        } else {
            // Circle
            auto p = ep1.x + 1i * ep1.y;
            auto q = ep2.x + 1i * ep2.y;

            auto o_ = 
                (p * (q * std::conj(q) + 1.) - q * (p * std::conj(p) + 1.)) /
            // ---------------------------------------------------------------
                        (p * std::conj(q) - q * std::conj(p));

            auto o = EPoint(std::real(o_), std::imag(o_));
            auto r = dist(o.getPos(), ep1.getPos());

            return new ECircle(o, r);
        }
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
}
