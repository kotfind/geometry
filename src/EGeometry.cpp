#include "EGeometry.h"

#include "AbstractPoint.h"
#include "Point.h"
#include "ETransformation.h"

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
