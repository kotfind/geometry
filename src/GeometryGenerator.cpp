#include "GeometryGenerator.h"

#include "GeometryItem.h"
#include "GeometryObject.h"
#include "Object.h"
#include "Geometry.h"

bool GeometryGenerator::isReal() const {
    return false;
}

GeometryItem* GeometryGenerator::getGeometryItem() const {
    return item.get();
}

bool GeometryGenerator::checkObjectType() const {
    return !obj->is(Type::Real);
}

void GeometryGenerator::beginResetObject() {
    item->beginResetObject();
}

void GeometryGenerator::endResetObject() {
    item->endResetObject();
}

void GeometryGenerator::move(const QPointF& delta) {
    beginResetObject();

    static_cast<GeometryObject*>(obj.get())->move(delta);

    endResetObject();
    geom->setChanged();

    recalc();
}
