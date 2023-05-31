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
        auto p = *static_cast<Point*>(pt->clone());
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
}
