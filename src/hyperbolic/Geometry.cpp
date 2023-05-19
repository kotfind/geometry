#include "Geometry.h"

#include "Transformation.h"
#include "Point.h"
#include "Line.h"

#include "core/Real.h"

#include "util/math.h"
#include "util/TR.h"

#include <stdexcept>

namespace hyperbolic {
    using namespace impl;

    Geometry::Geometry()
      : transformation(
            std::make_unique<Transformation>()
        ),
        sectionMaster(makeSectionMaster())
    {}

    AbstractPoint* Geometry::makePoint(const QPointF& pos) const {
        try {
            return new Point(pos);
        } catch (const std::invalid_argument&) {
            return nullptr;
        }
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
}
