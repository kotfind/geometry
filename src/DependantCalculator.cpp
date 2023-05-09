#include "DependantCalculator.h"

#include "Generator.h"
#include "Function.h"

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
