#include "Geometry.h"

#include "Transformation.h"
#include "Point.h"
#include "Line.h"
#include "Circle.h"
#include "Segment.h"

#include "core/AbstractPoint.h"
#include "core/Real.h"

#include "util/TR.h"

#include <QString>
#include <stdexcept>

namespace euclidian {
    using namespace impl;

    Geometry::Geometry()
      : transformation(
            std::make_unique<Transformation>()
        ),
        sectionMaster(makeSectionMaster())
    {}

    AbstractPoint* Geometry::makePoint(const QPointF& pos) const {
        return new Point(pos);
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
            case Circle::Type: return TR("Circle");
            case Segment::Type: return TR("Segment");
            default: throw std::invalid_argument("Wrong or complex type");
        }
    }
}
