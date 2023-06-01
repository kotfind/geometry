#include "Segment.h"

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
    Segment::Segment() {}

    Segment::Segment(const Point& p1, const Point& p2)
      : p1(p1),
        p2(p2)
    {}

    Object* Segment::clone() const {
        return new Segment(*this);
    }

    void Segment::paint(QPainter* qp, const QColor& color) const {
        auto pen = qp->pen();
        pen.setWidthF(paintWidth);
        pen.setColor(color);
        qp->setPen(pen);

        qp->drawLine(p1.getPos(), p2.getPos());
    }

    QRectF Segment::boundingRect() const {
        return QRectF(p1.getPos(), p2.getPos()).normalized();
    }

    QPainterPath Segment::shape() const {
        auto d = norm(*this) * paintWidth;

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
        // FIXME
        return new Point();
    }

    double Segment::pointToPosValue(const AbstractPoint* pt) const {
        // FIXME
        return 0;
    }

    AbstractPoint* Segment::posValueToPoint(double val) const {
        // FIXME
        return new Point();
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
}
