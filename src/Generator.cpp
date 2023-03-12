#include "Generator.h"

#include "Function.h"
#include "Object.h"
#include "GeometryObject.h"
#include "GeometryItem.h"
#include "Point.h"

Generator::Generator(Function* func, const QList<Generator*>& args, int funcResNum)
  : func(func),
    args(args),
    funcResNum(funcResNum)
{
    initItem();
    recalc();

    for (auto* gen : args) {
        gen->dependant << this;
    }
}

Generator::Generator(Object* obj)
  : object(obj)
{
    initItem();
}

void Generator::initItem() {
    item = new GeometryItem(this);
}

Generator::~Generator() {
    delete object;
}

void Generator::recalc() {
    recalcDependant();

    if (isFree()) return;

    QList<Object*> objs;
    objs.reserve(args.size());

    for (auto* gen : args) {
        objs << gen->object;
    }

    item->beginResetObject();

    delete object;
    const auto& res = (*func)(objs);
    object = funcResNum < res.size() ? res[funcResNum] : nullptr;

    item->beginResetObject();
}

bool Generator::isFree() const {
    return !func;
}

void Generator::recalcDependant() const {
    for (auto* dep : dependant) {
        dep->recalc();
    }
}

void Generator::setPos(double x, double y) {
    assert(isFree());
    auto* pt = dynamic_cast<Point*>(object);
    assert(pt);

    item->beginResetObject();
    pt->setPos(x, y);
    item->endResetObject();

    recalc();
}

int Generator::getObjectType() const {
    return object ? object->getType() : -1;
}
