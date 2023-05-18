#include "Geometry.h"

#include "AbstractPoint.h"
#include "Point.h"
#include "functionList.h"

Geometry::Geometry()
  : transformation(
        std::make_unique<Transformation>()
    ),
    sectionMaster(functionList::makeSectionMaster())
{}

AbstractPoint* Geometry::makePoint(const QPointF& pos) {
    return new Point(pos);
}

const SectionMaster* Geometry::getSectionMaster() const {
    return sectionMaster.get();
}

Transformation& Geometry::getTransformation() const {
    return *transformation;
}
