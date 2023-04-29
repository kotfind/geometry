#include "Generator.h"

#include "FreeCalculator.h"
#include "DependantCalculator.h"
#include "Object.h"
#include "Geometry.h"
#include "Point.h"
#include "Function.h"

#include <stdexcept>
#include <QJsonArray>

Generator::Generator(Object* obj)
  : calc(std::make_unique<FreeCalculator>()),
    obj(obj->clone())
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
    beginResetObject();

    obj.reset(calc->calc(obj.get()));
    if (obj && !checkObjectType()) throw std::runtime_error("Wrong object type");

    for (auto* dep : dependant) {
        dep->recalc();
    }

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

