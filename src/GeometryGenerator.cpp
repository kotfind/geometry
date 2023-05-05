#include "GeometryGenerator.h"

#include "GeometryItem.h"
#include "GeometryObject.h"
#include "Object.h"
#include "Geometry.h"
#include "Point.h"

bool GeometryGenerator::isReal() const {
    return false;
}

GeometryItem* GeometryGenerator::getGeometryItem() const {
    return item.get();
}

bool GeometryGenerator::checkObjectType() const {
    return isFree() && obj->is(Type::Point) ||
        !obj->is(Type::Real);
}

void GeometryGenerator::beginResetObject() {
    item->beginResetObject();
}

void GeometryGenerator::endResetObject() {
    item->endResetObject();
}

void GeometryGenerator::move(const QPointF& delta) {
    beginResetObject();

    static_cast<Point*>(obj.get())->move(delta);

    endResetObject();
    geom->setChanged();

    recalc();
}
