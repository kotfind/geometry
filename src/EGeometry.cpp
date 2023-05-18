#include "EGeometry.h"

#include "AbstractPoint.h"
#include "Point.h"
#include "ETransformation.h"
#include "TR.h"
#include "Real.h"
#include "Point.h"
#include "Line.h"
#include "Circle.h"

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
