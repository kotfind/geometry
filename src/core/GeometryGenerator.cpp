#include "GeometryGenerator.h"

#include "GeometryItem.h"
#include "GeometryObject.h"
#include "Object.h"
#include "Engine.h"
#include "FreeCalculator.h"

bool GeometryGenerator::isReal() const {
    return false;
}

GeometryItem* GeometryGenerator::getGeometryItem() const {
    return item.get();
}

const GeometryObject* GeometryGenerator::getGeometryObject() const {
    return static_cast<const GeometryObject*>(getObject());
}

bool GeometryGenerator::checkObjectType() const {
    if (obj->isReal()) return false;
    if ((isFree() || isRestricted()) &&
        !static_cast<GeometryObject*>(obj.get())->isPoint()) return false;
    return true;
}

void GeometryGenerator::setPos(const AbstractPoint* pos) {
    assert(isFree() || isRestricted());

    calc->setPos(pos);

    engine->setChanged();

    recalc();
}

void GeometryGenerator::onChanged() {
    Generator::onChanged();
    item->update();
}
