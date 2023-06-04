#include "Segment.h"

#include "Point.h"
#include "Line.h"
#include "config.h"

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
    Segment::Segment() {}

    Segment::Segment(const Point& p1, const Point& p2)
      : p1(p1),
        p2(p2)
    {}

    Object* Segment::clone() const {
        return new Segment(*this);
    }

    void Segment::paint(QPainter* qp, double scale, const QColor& color) const {
        auto pen = qp->pen();
        pen.setWidthF(scale * paintWidth);
        pen.setColor(color);
        qp->setPen(pen);

        qp->drawLine(p1.getPos(), p2.getPos());
    }

    QRectF Segment::boundingRect(double scale) const {
        return QRectF(p1.getPos(), p2.getPos()).normalized();
    }

    QPainterPath Segment::shape(double scale) const {
        auto d = norm(*this) * scale * paintWidth;

        QPainterPath path;
        path.moveTo((p1 - d).getPos());
        path.lineTo((p1 + d).getPos());
        path.lineTo((p2 + d).getPos());
        path.moveTo((p2 - d).getPos());
        return path;
    }

    void Segment::transform(const AbstractTransformation* t) {
        p1.transform(t);
        p2.transform(t);
    }

    AbstractPoint* Segment::calcNearestPoint(const AbstractPoint* pt) const {
        auto p = *static_cast<const Point*>(pt);

        if (le(dot(p - p1, p2 - p1), 0)) {
            return new Point(p1);
        }

        if (le(dot(p - p2, p1 - p2), 0)) {
            return new Point(p2);
        }

        return toLine().calcNearestPoint(pt);
    }

    double Segment::pointToPosValue(const AbstractPoint* pt) const {
        return toLine().pointToPosValue(pt);
    }

    AbstractPoint* Segment::posValueToPoint(double val) const {
        return toLine().posValueToPoint(val);
    }

    std::tuple<double, double, double> Segment::getABC() const {
        return toLine().getABC();
    }

    Line Segment::toLine() const {
        return Line(p1, p2);
    }

    Point norm(const Segment& l) {
        auto d = dir(l);
        return Point(d.y, -d.x);
    }

    Point dir(const Segment& l) {
        auto res = l.p2 - l.p1;
        res /= len(res);
        return res;
    }

    bool on(const Point& p, const Segment& s) {
        auto p1 = s.p1;
        auto p2 = s.p2;

        return
            eq(cross(p2 - p1, p - p1), 0) &&
            leq(dot(p1 - p, p2 - p), 0);
    }
}
