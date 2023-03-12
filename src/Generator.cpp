#include "Generator.h"

#include "Function.h"
#include "Object.h"
#include "GeometryObject.h"
#include "GeometryItem.h"

Generator::Generator(Function* func, const QList<Generator*>& args, int funcResNum)
  : func(func),
    args(args),
    funcResNum(funcResNum)
{
    initItem();
    recalc();
}

Generator::Generator(Object* obj)
  : object(obj)
{
    initItem();
}

void Generator::initItem() {
    item = new GeometryItem(this);

    auto flags = item->flags();
    flags |= QGraphicsItem::ItemIsFocusable;
    if (isFree()) {
        flags |= QGraphicsItem::ItemIsMovable;
    }
    item->setFlags(flags);
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

        item->beginResetObject();

        const auto& res = (*func)(objs);
        object = funcResNum < res.size() ? res[funcResNum] : nullptr;

        item->beginResetObject();
    }
}

bool Generator::isFree() const {
    return !func;
}
