#include "Line.h"

#include "Point.h"

#include "core/AbstractTransformation.h"

#include "util/math.h"

#include <cassert>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

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
        auto proj = getEuclidian();

        if (std::holds_alternative<ELine>(proj)) {
            std::get<ELine>(proj).paint(qp, color);
        } else {
            std::get<ECircle>(proj).paint(qp, color);
        }
    }

    QRectF Line::boundingRect() const {
        auto proj = getEuclidian();

        if (std::holds_alternative<ELine>(proj)) {
            return std::get<ELine>(proj).boundingRect();
        } else {
            return std::get<ECircle>(proj).boundingRect();
        }
    }

    QPainterPath Line::shape() const {
        auto proj = getEuclidian();

        if (std::holds_alternative<ELine>(proj)) {
            return std::get<ELine>(proj).shape();
        } else {
            return std::get<ECircle>(proj).shape();
        }
    }

    void Line::transform(const AbstractTransformation*) {
        // FIXME
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

    std::variant<ECircle, ELine> Line::getEuclidian() const
    {
        // FIXME: QPointF -> Point ?
        if (abs(cross(p1.getPos(), p2.getPos()) / dist(p1.getPos(), p2.getPos())) < 0.01 /* XXX */) {
            // Line
            auto p = p1.getPos();
            auto d = p2.getPos() - p1.getPos();

            // Solving equation system:
            // x^2 + y^2 = 1
            // x = p_x + t * d_x
            // y = p_y + t * d_y

            int n;
            double t1, t2;
            solveSqEq(
                sq(d.x()) + sq(d.y()), // a
                2 * (d.x() * p.x() + d.y() * p.y()),  // b
                sq(p.x()) + sq(p.y()) - 1, // c
                n,
                t1,
                t2
            );
            assert(n == 2);

            return ELine(
                EPoint(p + t1 * d),
                EPoint(p + t2 * d)
            );
        } else {
            // Circle
            auto p = p1.getComplex();
            auto q = p2.getComplex();

            auto o_ = 
                (p * (q * std::conj(q) + 1.) - q * (p * std::conj(p) + 1.)) /
            // ---------------------------------------------------------------
                        (p * std::conj(q) - q * std::conj(p));

            auto o = EPoint(std::real(o_), std::imag(o_));
            auto r = dist(o.getPos(), p1.getPos());

            return ECircle(o, r);
        }
    }
}
