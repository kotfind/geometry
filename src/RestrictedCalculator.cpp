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
    return new Point(
        restrictor->getGeometryObject()->calcNearestPoint(mousePos)
    );
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
