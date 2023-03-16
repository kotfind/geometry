#include "DependantGenerator.h"

#include "Function.h"
#include "Object.h"

#include <QJsonObject>
#include <QJsonArray>

DependantGenerator::DependantGenerator(Geometry* geom, Function* func, const QList<Generator*>& args, int funcResNum)
  : Generator(geom),
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
        objs << gen->getObject();
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
