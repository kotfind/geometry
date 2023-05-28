#include "Arc.h"

#include "Point.h"
#include "Line.h"

#include "core/AbstractTransformation.h"

#include "util/math.h"

#include <QRectF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <math.h>
#include <QPointF>
#include <memory>

namespace euclidian::impl {
    Arc::Arc() {}

    Arc::Arc(const Point& o, const Point& p1, const Point& p2)
      : o(o),
        p1(p1),
        p2(p2)
    {}

    Object* Arc::clone() const {
        return new Arc(*this);
    }

    void Arc::paint(QPainter* qp, const QColor& color) const {
        auto pen = qp->pen();
        pen.setWidthF(paintWidth);
        pen.setColor(color);
        qp->setPen(pen);

        qp->drawPath(getPainterPath());
    }

    QRectF Arc::boundingRect() const {
        return getPainterPath().boundingRect();
    }

    QPainterPath Arc::shape() const {
        // FIXME
        return getPainterPath();
    }

    void Arc::transform(const AbstractTransformation* t) {
        p1.transform(t);
        p2.transform(t);
    }

    AbstractPoint* Arc::calcNearestPoint(const AbstractPoint* pt) const {
        // FIXME
        return new Point();
    }

    double Arc::pointToPosValue(const AbstractPoint* pt) const {
        // FIXME
        return 0;
    }

    AbstractPoint* Arc::posValueToPoint(double val) const {
        // FIXME
        return new Point();
    }

    QPainterPath Arc::getPainterPath() const {
        auto a = p1 - o;
        auto b = p2 - o;
        auto k = (4./3) * (sqrt(2) * len(a) * len(b) - sq(len(b))) / cross(a, b);
        auto m1 = o + a + k * Point(-a.y, a.x);
        auto m2 = o + b + k * Point(b.y, -b.x);

        QPainterPath path;
        path.moveTo(
            p1.getPos()
        );
        path.cubicTo(
            m1.getPos(),
            m2.getPos(),
            p2.getPos()
        );
        return path;
    }
}
