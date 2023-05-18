#include "Generator.h"

#include "FreeCalculator.h"
#include "DependantCalculator.h"
#include "RestrictedCalculator.h"
#include "Object.h"
#include "Engine.h"
#include "Function.h"
#include "GeometryGenerator.h"
#include "RealGenerator.h"
#include "Real.h"
#include "SectionMaster.h"

#include "util/getOrThrow.h"

#include <stdexcept>
#include <QJsonArray>

Generator::Generator(const Geometry* geom, std::unique_ptr<Object> obj)
  : Generator(
        std::make_unique<FreeCalculator>(
            geom,
            std::move(obj)
        )
    )
{}

Generator::Generator(
    const Geometry* geom,
    const Function* func,
    const QList<Generator*>& args,
    int funcResNum
) : Generator(
        std::make_unique<DependantCalculator>(
            geom,
            func,
            args,
            funcResNum
        )
    )
{}

Generator::Generator(const Geometry* geom, GeometryGenerator* restrictor, double posValue)
  : Generator(
        std::make_unique<RestrictedCalculator>(
            geom,
            restrictor,
            posValue
        )
    )
{}

Generator::Generator(std::unique_ptr<Calculator> calc)
  : calc(std::move(calc))
{
    for (auto* gen : getArgs()) {
        gen->dependant << this;
    }
}

const Object* Generator::getObject() const {
    return obj.get();
}

bool Generator::isFree() const {
    return calc->getType() == Calculator::Type::FREE;
}

bool Generator::isDependant() const {
    return calc->getType() == Calculator::Type::DEPENDANT;
}

bool Generator::isRestricted() const {
    return calc->getType() == Calculator::Type::RESTRICTED;
}

bool Generator::isGeometry() const {
    return !isReal();
}

void Generator::recalc() {
    recalcSelf();

    for (auto* dep : dependant) {
        dep->recalc();
    }
}

void Generator::recalcSelf() {
    obj.reset(calc->calc());
    if (obj && !checkObjectType()) throw std::runtime_error("Wrong object type");

    onChanged();
}

QList<Generator*> Generator::getArgs() const {
    return calc->getArgs();
}

Engine* Generator::getEngine() const {
    return engine;
}

QJsonObject Generator::toJson(const QHash<Generator*, int>& ids) const {
    QJsonObject json;

    json["isReal"] = isReal();

    json["calc"] = calc->toJson(ids, isReal());

    if (isReal()) {
        // TODO: Move to RealGenerator::toJson ?
        json["name"] = static_cast<const RealGenerator*>(this)->getName();
    } else {
        // TODO: Move to GeometryGenerator::toJson
        json["isHidden"] = static_cast<const GeometryGenerator*>(this)->getGeometryItem()->isHidden();
    }

    return json;
}

Generator* Generator::fromJson(
    const Geometry* geom,
    const QJsonObject& json,
    const QList<Generator*>& gens
) {
    auto isReal = getOrThrow(json["isReal"]).toBool();

    auto calc = std::unique_ptr<Calculator>(
        Calculator::fromJson(
            geom,
            getOrThrow(json["calc"]).toObject(),
            gens,
            isReal
        )
    );

    if (isReal) {
        // TODO: Move to RealGenerator::fromJson
        auto name = getOrThrow(json["name"]).toString();
        return new RealGenerator(name, std::move(calc));
    } else {
        // TODO: Move to GeometryGenerator::fromJson
        auto isHidden = getOrThrow(json["isHidden"]).toBool();

        auto* gen = new GeometryGenerator(std::move(calc));
        gen->getGeometryItem()->setHidden(isHidden);

        return gen;
    }
}

void Generator::onChanged() {
    engine->generatorChanged(this);
}
