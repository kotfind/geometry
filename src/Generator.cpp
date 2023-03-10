#include "Generator.h"

#include "Function.h"
#include "Object.h"
#include "GeometryObject.h"
#include "GeometryItem.h"

Generator::Generator(Function* func, const QList<Generator*>& args, int funcResNum)
  : func(func),
    args(args),
    funcResNum(funcResNum),
    item(new GeometryItem)
{
    recalc();
}

Generator::Generator(Object* obj)
  : object(obj),
    item(new GeometryItem)
{
    recalc();
}

Generator::~Generator() {
    delete object;
}

void Generator::recalc() {
    if (!isFree()) {
        QList<Object*> objs;
        objs.reserve(args.size());

        for (auto* gen : args) {
            objs << gen->object;
        }

        const auto& res = (*func)(objs);
        object = funcResNum < res.size() ? res[funcResNum] : nullptr;
    }

    item->setObject(dynamic_cast<GeometryObject*>(object));
}

bool Generator::isFree() const {
    return !func;
}
