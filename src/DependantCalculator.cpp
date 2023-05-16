#include "DependantCalculator.h"

#include "Generator.h"
#include "Function.h"
#include "getOrThrow.h"
#include "SectionMaster.h"

#include <QJsonArray>

DependantCalculator::DependantCalculator(
    const Function* func,
    const QList<Generator*>& args,
    int funcResNum
) : func(func),
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

const QList<Generator*> DependantCalculator::getArgs() const {
    return args;
}

const Function* DependantCalculator::getFunc() const {
    return func;
}

int DependantCalculator::getFuncResNum() const {
    return funcResNum;
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
    const QJsonObject& json,
    const QList<Generator*>& gens,
    const SectionMaster* sectionMaster
) {
    const auto& funcName = getOrThrow(json["funcName"]).toString();
    const auto* func = sectionMaster->get(funcName);

    QList<Generator*> args;
    const auto& jsonArgs = getOrThrow(json["args"]).toArray();
    for (const auto& arg : jsonArgs) {
        int id = arg.toInt();
        args << gens[id];
    }

    auto funcResNum = getOrThrow(json["funcResNum"]).toInt();

    return new DependantCalculator(func, args, funcResNum);
}
