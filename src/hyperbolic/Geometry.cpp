#include "Geometry.h"

#include "Transformation.h"
#include "Point.h"
#include "Line.h"

#include "core/Real.h"

#include "util/math.h"
#include "util/TR.h"

#include <QPen>

using EPoint = euclidian::impl::Point;

namespace hyperbolic {
    using namespace impl;

    Geometry::Geometry()
      : transformation(
            std::make_unique<Transformation>()
        ),
        sectionMaster(makeSectionMaster())
    {
        absoluteCircleItem = std::make_unique<QGraphicsEllipseItem>(-1, -1, 2, 2);
        auto pen = absoluteCircleItem->pen();
        pen.setWidth(2);
        pen.setCosmetic(true);
        pen.setStyle(Qt::PenStyle::SolidLine);
        pen.setColor(Qt::red);
        absoluteCircleItem->setPen(pen);
    }

    AbstractPoint* Geometry::makePoint(const QPointF& pos) const {
        auto pt = EPoint(pos);

        if (geq(len(pos), 0.99)) {
            pt *= 0.99 / len(pos);
        }

        return new Point(pt);
    }

    const SectionMaster* Geometry::getSectionMaster() const {
        return sectionMaster.get();
    }

    AbstractTransformation* Geometry::getTransformation() const {
        return transformation.get();
    }

    QString Geometry::typeName(int t) const {
        switch (t) {
            case Real::Type: return TR("Real");
            case Point::Type: return TR("Point");
            case Line::Type: return TR("Line");
            // case Circle::Type: return TR("Circle");
            default: throw std::invalid_argument("Wrong or complex type");
        }
    }

    QGraphicsItem* Geometry::getGraphicsItem() const {
        return absoluteCircleItem.get();
    }
}
