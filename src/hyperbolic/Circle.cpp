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
}
