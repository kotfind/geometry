#include "DependantCalculator.h"

#include "Generator.h"
#include "Function.h"

DependantCalculator::DependantCalculator(
    Function* func,
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
    return funcResNum < res.size()
        ? res[funcResNum]
        : nullptr;
}

bool DependantCalculator::isFree() const {
    return false;
}
