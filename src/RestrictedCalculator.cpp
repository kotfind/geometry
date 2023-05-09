#include "RestrictedCalculator.h"

#include "GeometryGenerator.h"
#include "Point.h"

RestrictedCalculator::RestrictedCalculator(
    GeometryGenerator* restrictorGen,
    const QPointF& mousePos
) : restrictorGen(restrictorGen),
    mousePos(mousePos)
{}

Object* RestrictedCalculator::calc() const {
    return new Point(
        restrictorGen->getGeometryObject()->calcNearestPoint(mousePos)
    );
}

Calculator::Type RestrictedCalculator::getType() const {
    return Type::RESTRICTED;
}

void RestrictedCalculator::setPos(const QPointF& pos) {
    mousePos = pos;
}
