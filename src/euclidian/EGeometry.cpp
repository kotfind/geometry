#include "EGeometry.h"

#include "Point.h"
#include "ETransformation.h"
#include "Point.h"
#include "Line.h"
#include "Circle.h"

#include "core/AbstractPoint.h"
#include "core/Real.h"

#include "util/TR.h"

#include <QString>
#include <stdexcept>

EGeometry::EGeometry()
  : transformation(
        std::make_unique<ETransformation>()
    ),
    sectionMaster(makeSectionMaster())
{}

AbstractPoint* EGeometry::makePoint(const QPointF& pos) const {
    return new Point(pos);
}

const SectionMaster* EGeometry::getSectionMaster() const {
    return sectionMaster.get();
}

Transformation* EGeometry::getTransformation() const {
    return transformation.get();
}

QString EGeometry::typeName(int t) const {
    switch (t) {
        case Real::Type: return TR("Real");
        case Point::Type: return TR("Point");
        case Line::Type: return TR("Line");
        case Circle::Type: return TR("Circle");
        default: throw std::invalid_argument("Wrong or complex type");
    }
}
