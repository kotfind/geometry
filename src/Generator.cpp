#include "Generator.h"

#include "Function.h"
#include "Object.h"

Generator::Generator(Function* func, const QList<Generator*>& args, int funcResNum)
  : func(func),
    args(args),
    funcResNum(funcResNum)
{
    recalc();
}

Generator::~Generator() {
    delete object;
}

void Generator::recalc() {
    QList<Object*> objs;
    objs.reserve(args.size());

    for (auto* gen : args) {
        objs << gen->object;
    }

    object = (*func)(objs)[funcResNum];
}
