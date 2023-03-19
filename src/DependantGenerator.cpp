#include "DependantGenerator.h"

#include "Function.h"
#include "Object.h"
#include "getOrThrow.h"

#include <QJsonObject>
#include <QJsonArray>

DependantGenerator::DependantGenerator(Function* func, const QList<Generator*>& args, int funcResNum)
  : Generator(),
    func(func),
    args(args),
    funcResNum(funcResNum)
{
    recalc();

    for (auto* gen : args) {
        gen->addDependant(this);
    }
}

void DependantGenerator::recalcSelf() {
    QList<const Object*> objs;
    objs.reserve(args.size());

    for (auto* gen : args) {
        auto* obj = gen->getObject();
        if (!obj) {
            delete object;
            object = nullptr;
            return;
        }
        objs << obj;
    }

    delete object;
    const auto& res = (*func)(objs);
    object = funcResNum < res.size() ? res[funcResNum] : nullptr;
}

void DependantGenerator::remove() {
    for (auto* arg : args) {
        arg->removeDependant(this);
    }
    Generator::remove();
}

QJsonObject DependantGenerator::toJson(const QHash<Generator*, int>& ids) const {
    auto json = Generator::toJson(ids);
    json["funcName"] = func->getName();
    json["funcResNum"] = funcResNum;
    QJsonArray jsonArgs;
    for (auto arg : args) {
        jsonArgs << ids[arg];
    }
    json["args"] = jsonArgs;
    return json;
}

void DependantGenerator::load(Geometry* geom, const QJsonArray& jsonGens, QList<Generator*>& gens, int i) {
    const auto& json = jsonGens[i];

    const auto& funcName = getOrThrow(json["funcName"]).toString();
    auto* func = Function::get(funcName);

    QList<Generator*> args;
    const auto& jsonArgs = getOrThrow(json["args"]).toArray();
    for (const auto& arg : jsonArgs) {
        int id = arg.toInt();
        if (!gens[id]) {
            Generator::load(geom, jsonGens, gens, id);
        }
        args << gens[id];
    }

    auto funcResNum = getOrThrow(json["funcResNum"]).toInt();

    gens[i] = new DependantGenerator(func, args, funcResNum);
    gens[i]->setGeometry(geom);
}
