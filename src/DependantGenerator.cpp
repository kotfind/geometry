#include "DependantGenerator.h"

#include "Function.h"
#include "Object.h"

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
    QList<Object*> objs;
    objs.reserve(args.size());

    for (auto* gen : args) {
        objs << gen->getObject();
    }

    delete object;
    const auto& res = (*func)(objs);
    object = funcResNum < res.size() ? res[funcResNum] : nullptr;
}
