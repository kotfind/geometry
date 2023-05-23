#include "FreeCalculator.h"

#include "Real.h"
#include "AbstractPoint.h"
#include "AbstractGeometry.h"

#include "util/getOrThrow.h"

FreeCalculator::FreeCalculator(const AbstractGeometry* geom, std::unique_ptr<Object> obj)
  : Calculator(geom),
    obj(std::move(obj))
{}

Object* FreeCalculator::calc() const {
    return obj->clone();
}

Calculator::Type FreeCalculator::getType() const {
    return Type::FREE;
}

QList<Generator*> FreeCalculator::getArgs() const {
    return {};
}

void FreeCalculator::setPos(const AbstractPoint* pos) {
    assert(obj->isGeometry());
    assert(static_cast<GeometryObject*>(obj.get())->isPoint());
    obj.reset(pos->clone());
}

void FreeCalculator::setValue(double v) {
    assert(obj->isReal());
    static_cast<Real*>(obj.get())->value = v;
}

QJsonObject FreeCalculator::toJson(const QHash<Generator*, int>& ids, bool isReal) const {
    QJsonObject json;
    json["type"] = "FREE";

    json["object"] = isReal
        ? static_cast<Real*>(obj.get())->toJson()
        : static_cast<AbstractPoint*>(obj.get())->toJson();

    return json;
}

FreeCalculator* FreeCalculator::fromJson(
    const AbstractGeometry* geom,
    const QJsonObject& json,
    bool isReal
) {
    auto jsonObject = getOrThrow(json["object"]).toObject();
    std::unique_ptr<Object> obj;
    if (isReal) {
        obj.reset(Real::fromJson(jsonObject));
    } else {
        auto* pt = geom->makePoint();
        pt->fromJson(jsonObject);
        obj.reset(pt);
    }
    return new FreeCalculator(geom, std::move(obj));
}
