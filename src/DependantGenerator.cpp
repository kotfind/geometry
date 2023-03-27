#include "DependantGenerator.h"

#include "Function.h"
#include "Object.h"

DependantGenerator::DependantGenerator(
    Function* func,
    const QList<Generator*>& args,
    int funcResNum,
    Geometry* geom
) : Generator(geom),
    func(func),
    args(args),
    funcResNum(funcResNum)
{
    for (auto* gen : args) {
        gen->addDependant(this);
    }
}

void DependantGenerator::remove() {
    for (auto* arg : args) {
        arg->removeDependant(this);
    }
    Generator::remove();
}

void DependantGenerator::recalcSelf() {
    QList<const Object*> objs;
    objs.reserve(args.size());

    for (auto* gen : args) {
        auto* obj = gen->getObject();
        if (!obj) {
            delete object();
            object() = nullptr;
            return;
        }
        objs << obj;
    }

    delete object();
    const auto& res = (*func)(objs);
    object() = funcResNum < res.size() ? res[funcResNum] : nullptr;
}
