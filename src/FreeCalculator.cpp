#include "FreeCalculator.h"

#include "Real.h"
#include "getOrThrow.h"
#include "AbstractPoint.h"

FreeCalculator::FreeCalculator(std::unique_ptr<Object> obj)
  : obj(std::move(obj))
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

void FreeCalculator::setPos(const QPointF& pos) {
    assert(obj->isGeometry());
    assert(static_cast<GeometryObject*>(obj.get())->isPoint());
    static_cast<AbstractPoint*>(obj.get())->setPos(pos);
}

void FreeCalculator::setValue(double v) {
    assert(obj->is(Object::Type::Real));
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

FreeCalculator* FreeCalculator::fromJson(const QJsonObject& json, bool isReal) {
    auto jsonObject = getOrThrow(json["object"]).toObject();
    std::unique_ptr<Object> obj;
    if (isReal) {
        obj.reset(Real::fromJson(jsonObject));
    } else {
        auto* pt = new Point;
        pt->fromJson(jsonObject);
        obj.reset(pt);
    }
    return new FreeCalculator(std::move(obj));
}
