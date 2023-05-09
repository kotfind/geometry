#include "RestrictedCalculator.h"

#include "GeometryGenerator.h"
#include "Point.h"

RestrictedCalculator::RestrictedCalculator(
    GeometryGenerator* restrictor,
    const QPointF& mousePos
) : restrictor(restrictor),
    mousePos(mousePos)
{}

Object* RestrictedCalculator::calc() const {
    auto* obj = restrictor->getGeometryObject();
    return obj
        ? new Point(obj->calcNearestPoint(mousePos))
        : nullptr;
}

Calculator::Type RestrictedCalculator::getType() const {
    return Type::RESTRICTED;
}

void RestrictedCalculator::setPos(const QPointF& pos) {
    mousePos = pos;
}

GeometryGenerator* RestrictedCalculator::getRestrictor() const {
    return restrictor;
}
