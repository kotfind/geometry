#include "RestrictedCalculator.h"

#include "GeometryGenerator.h"
#include "Point.h"
#include "getOrThrow.h"

RestrictedCalculator::RestrictedCalculator(
    GeometryGenerator* restrictor,
    double posValue
) : restrictor(restrictor),
    posValue(posValue)
{}

Object* RestrictedCalculator::calc() const {
    auto* obj = restrictor->getGeometryObject();
    return obj
        ? new Point(obj->posValueToPoint(posValue))
        : nullptr;
}

Calculator::Type RestrictedCalculator::getType() const {
    return Type::RESTRICTED;
}

QList<Generator*> RestrictedCalculator::getArgs() const {
    return { restrictor };
}

void RestrictedCalculator::setPos(const QPointF& pos) {
    auto* obj = restrictor->getGeometryObject();
    if (!obj) return;

    auto nearest = obj->calcNearestPoint(pos);
    posValue = obj->pointToPosValue(nearest);
}

QJsonObject RestrictedCalculator::toJson(const QHash<Generator*, int>& ids, bool isReal) const {
    QJsonObject json;
    json["type"] = "RESTRICTED";

    json["restrictor"] = ids[restrictor];
    json["posValue"] = posValue;

    return json;
}

RestrictedCalculator* RestrictedCalculator::fromJson(
    const QJsonObject& json,
    const QList<Generator*>& gens
) {
    auto* restrictor = gens[getOrThrow(json["restrictor"]).toInt()];
    auto posValue = getOrThrow(json["posValue"]).toDouble();

    assert(restrictor->isGeometry());
    return new RestrictedCalculator(
        static_cast<GeometryGenerator*>(restrictor),
        posValue
    );
}
