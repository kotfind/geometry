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
        auto pen = qp->pen();
        pen.setWidthF(paintWidth);
        pen.setStyle(Qt::SolidLine);
        pen.setColor(color);
        qp->setPen(pen);

        bool isLine;
        // for line
        Point a;
        Point b;
        // for circle
        Point o;
        double r;
        getEuclidian(isLine, a, b, o, r);

        if (isLine) {
            qp->drawLine(a.getPos(), b.getPos());
        } else {
            qp->drawEllipse(getCircleRect(o.getPos(), r));
        }
    }

    QRectF Line::boundingRect() const {
        bool isLine;
        // for line
        Point a;
        Point b;
        // for circle
        Point o;
        double r;
        getEuclidian(isLine, a, b, o, r);

        if (isLine) {
            auto [x1, y1] = a.getPos();
            auto [x2, y2] = b.getPos();
            return QRectF(
                std::min(x1, x2) - paintWidth,
                std::min(y1, y2) - paintWidth,
                abs(x1 - x2)     + paintWidth * 2,
                abs(y1 - y2)     + paintWidth * 2
            );
        } else {
            return getCircleRect(o.getPos(), r + paintWidth);
        }
    }

    QPainterPath Line::shape() const {
        bool isLine;
        // for line
        Point a;
        Point b;
        // for circle
        Point o;
        double r;
        getEuclidian(isLine, a, b, o, r);

        if (isLine) {
            // TODO: put get norm into function
            QPointF norm(
                b.y - a.y,
                a.x - b.x
            );
            norm /= len(norm);

            auto d = norm * paintWidth;

            QPainterPath path;
            path.moveTo(a.getPos() - d);
            path.lineTo(a.getPos() + d);
            path.lineTo(b.getPos() + d);
            path.moveTo(b.getPos() - d);
            return path;
        } else {
            QPainterPath path;
            path.addEllipse(getCircleRect(o.getPos(), r + paintWidth));
            path.addEllipse(getCircleRect(o.getPos(), r - paintWidth));
            return path;
        }
    }

    GeometryObject* Line::transformed(const AbstractTransformation*) const {
        // FIXME
        return static_cast<GeometryObject*>(clone());
    }

    QPointF Line::calcNearestPoint(const QPointF& pos) const {
        // FIXME
        return QPointF();
    }

    double Line::pointToPosValue(const QPointF& pos) const {
        // FIXME
        return 0;
    }

    QPointF Line::posValueToPoint(double val) const {
        // FIXME
        return QPointF();
    }

    void Line::getEuclidian(
        bool& isLine,

        // For line
        Point& a,
        Point& b,

        // For circle
        Point& o,
        double& r
    ) const {
        if (eq(cross(p1.getPos(), p2.getPos()) / dist(p1.getPos(), p2.getPos()), 0)) {
            isLine = true;

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
                d.x() * p.x() + d.y() * p.y(),  // b
                sq(p.x()) + sq(p.y()) - 1, // c
                n,
                t1,
                t2
            );
            assert(n == 2);
            a = Point(p + t1 * d);
            b = Point(p + t2 * d);

        } else {
            isLine = false;

            auto p = p1.getComplex();
            auto q = p2.getComplex();

            o = Point(
                (p * (q * std::conj(q) + 1.) - q * (p * std::conj(p) + 1.)) /
            // ---------------------------------------------------------------
                        (p * std::conj(q) - q * std::conj(p))
            );
            r = dist(o.getPos(), p1.getPos());
        }
    }
}
