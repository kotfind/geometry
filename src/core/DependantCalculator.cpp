#include "DependantCalculator.h"

#include "Generator.h"
#include "Function.h"
#include "SectionMaster.h"
#include "AbstractGeometry.h"

#include "util/getOrThrow.h"

#include <QJsonArray>

DependantCalculator::DependantCalculator(
    const AbstractGeometry* geom,
    const Function* func,
    const QList<Generator*>& args,
    int funcResNum
) : Calculator(geom),
    func(func),
    args(args),
    funcResNum(funcResNum)
{}

Object* DependantCalculator::calc() const {
    QList<const Object*> objs;
    objs.reserve(args.size());

    for (auto* gen : args) {
        auto* obj = gen->getObject();
        if (!obj) return nullptr;
        objs << obj;
    }

    const auto& res = (*func)(objs);
    // Delete redundant objects
    for (int i = 0; i < res.size(); ++i) {
        if (i != funcResNum) {
            delete res[i];
        }
    }

    return funcResNum < res.size()
        ? res[funcResNum]
        : nullptr;
}

Calculator::Type DependantCalculator::getType() const {
    return Type::DEPENDANT;
}

QList<Generator*> DependantCalculator::getArgs() const {
    return args;
}

QJsonObject DependantCalculator::toJson(const QHash<Generator*, int>& ids, bool isReal) const {
    QJsonObject json;
    json["type"] = "DEPENDANT";

    json["funcName"] = func->getFullName();
    json["funcResNum"] = funcResNum;
    QJsonArray jsonArgs;
    for (auto arg : args) {
        jsonArgs << ids[arg];
    }
    json["args"] = jsonArgs;

    return json;
}

DependantCalculator* DependantCalculator::fromJson(
    const AbstractGeometry* geom,
    const QJsonObject& json,
    const QList<Generator*>& gens
) {
    const auto& funcName = getOrThrow(json["funcName"]).toString();
    const auto* func = geom->getSectionMaster()->get(funcName);

    QList<Generator*> args;
    const auto& jsonArgs = getOrThrow(json["args"]).toArray();
    for (const auto& arg : jsonArgs) {
        int id = arg.toInt();
        args << gens[id];
    }

    auto funcResNum = getOrThrow(json["funcResNum"]).toInt();

    return new DependantCalculator(geom, func, args, funcResNum);
}
