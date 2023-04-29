#include "Generator.h"

#include "FreeCalculator.h"
#include "DependantCalculator.h"
#include "Object.h"
#include "Geometry.h"
#include "Point.h"
#include "Function.h"
#include "getOrThrow.h"
#include "GeometryGenerator.h"

#include <stdexcept>
#include <QJsonArray>

Generator::Generator(std::unique_ptr<Object> obj)
  : calc(std::make_unique<FreeCalculator>()),
    obj(std::move(obj))
{}

Generator::Generator(
    Function* func,
    const QList<Generator*>& args,
    int funcResNum
) : calc(
        std::make_unique<DependantCalculator>(
            func,
            args,
            funcResNum
        )
    )
{
    for (auto* gen : args) {
        gen->dependant << this;
    }
}

const Object* Generator::getObject() const {
    return obj.get();
}

bool Generator::isFree() const {
    return calc->isFree();
}

bool Generator::isDependant() const {
    return !isFree();
}

void Generator::recalc() {
    recalcSelf();

    for (auto* dep : dependant) {
        dep->recalc();
    }
}

void Generator::recalcSelf() {
    beginResetObject();

    obj.reset(calc->calc(obj.get()));
    if (obj && !checkObjectType()) throw std::runtime_error("Wrong object type");

    endResetObject();
}

QJsonObject Generator::toJson(const QHash<Generator*, int>& ids) const {
    QJsonObject json;

    json["isFree"] = isFree();

    if (isFree()) {
        json["object"] = static_cast<Point*>(obj.get())->toJson();
    } else {
        auto* depCalc = static_cast<DependantCalculator*>(calc.get());

        json["funcName"] = depCalc->getFunc()->getFullName();
        json["funcResNum"] = depCalc->getFuncResNum();
        QJsonArray jsonArgs;
        for (auto arg : depCalc->getArgs()) {
            jsonArgs << ids[arg];
        }
        json["args"] = jsonArgs;
    }

    return json;
}

Generator* Generator::fromJson(const QJsonObject& json, const QList<Generator*>& gens) {
    auto isFree = getOrThrow(json["isFree"]).toBool();

    Generator* gen;

    if (isFree) {
        auto* pt = Point::fromJson(getOrThrow(json["object"]).toObject());
        gen = new GeometryGenerator(std::unique_ptr<Point>(pt));
    } else {
        const auto& funcName = getOrThrow(json["funcName"]).toString();
        auto* func = Function::get(funcName);

        QList<Generator*> args;
        const auto& jsonArgs = getOrThrow(json["args"]).toArray();
        for (const auto& arg : jsonArgs) {
            int id = arg.toInt();
            args << gens[id];
        }

        auto funcResNum = getOrThrow(json["funcResNum"]).toInt();

        gen = new GeometryGenerator(func, args, funcResNum);
    }

    return gen;
}
