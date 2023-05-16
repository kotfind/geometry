#include "Generator.h"

#include "FreeCalculator.h"
#include "DependantCalculator.h"
#include "RestrictedCalculator.h"
#include "Object.h"
#include "Geometry.h"
#include "Point.h"
#include "Function.h"
#include "getOrThrow.h"
#include "GeometryGenerator.h"
#include "RealGenerator.h"
#include "Real.h"
#include "SectionMaster.h"

#include <stdexcept>
#include <QJsonArray>

Generator::Generator(std::unique_ptr<Object> obj)
  : Generator(
        std::make_unique<FreeCalculator>(
            std::move(obj)
        )
    )
{}

Generator::Generator(
    const Function* func,
    const QList<Generator*>& args,
    int funcResNum
) : Generator(
        std::make_unique<DependantCalculator>(
            func,
            args,
            funcResNum
        )
    )
{}

Generator::Generator(GeometryGenerator* restrictor, double posValue)
  : Generator(
        std::make_unique<RestrictedCalculator>(
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
    if (isRestricted()) {
        return { static_cast<RestrictedCalculator*>(calc.get())->getRestrictor() };
    } else if (isDependant()) {
        return static_cast<DependantCalculator*>(calc.get())->getArgs();
    } else /* isFree() */ {
        return {};
    }
}

Geometry* Generator::getGeometry() const {
    return geom;
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
    const QJsonObject& json,
    const QList<Generator*>& gens,
    const SectionMaster* sectionMaster
) {
    auto isReal = getOrThrow(json["isReal"]).toBool();

    auto calc = std::unique_ptr<Calculator>(
        Calculator::fromJson(
            getOrThrow(json["calc"]).toObject(),
            gens,
            sectionMaster,
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
    geom->generatorChanged(this);
}
