#include "Generator.h"

#include "Function.h"
#include "Object.h"
#include "GeometryObject.h"
#include "GeometryItem.h"
#include "Point.h"
#include "Geometry.h"

#include <QPointF>

Generator::Generator(Geometry* geom, Function* func, const QList<Generator*>& args, int funcResNum)
  : geom(geom),
    func(func),
    args(args),
    funcResNum(funcResNum)
{
    init();
    recalc();

    for (auto* gen : args) {
        gen->dependant << this;
    }
}

Generator::Generator(Geometry* geom, Object* obj)
  : geom(geom),
    origObject(obj),
    object(new Point(*dynamic_cast<Point*>(obj))) // XXX
{
    init();
}

void Generator::init() {
    geom->addGenerator(this);
    item = new GeometryItem(this);
}

Generator::~Generator() {
    delete object;
    delete origObject;
}

void Generator::recalc() {
    if (isFree()) {
        auto* origPt = dynamic_cast<Point*>(origObject); // XXX
        auto* pt = dynamic_cast<Point*>(object); // XXX
        pt->setPos(geom->transform(origPt->getPos()));
    } else {
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

    recalcDependant();
}

bool Generator::isFree() const {
    return !func;
}

void Generator::recalcDependant() const {
    for (auto* dep : dependant) {
        dep->recalc();
    }
}

void Generator::move(const QPointF& delta) {
    assert(isFree());
    auto* pt = dynamic_cast<Point*>(origObject); // XXX
    assert(pt);

    item->beginResetObject();
    pt->move(delta);
    item->endResetObject();

    recalc();
}

int Generator::getObjectType() const {
    return object ? object->getType() : -1;
}
