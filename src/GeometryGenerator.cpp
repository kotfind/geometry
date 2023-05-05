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

const GeometryObject* GeometryGenerator::getGeometryObject() const {
    return static_cast<const GeometryObject*>(getObject());
}

bool GeometryGenerator::checkObjectType() const {
    return isFree() && obj->is(Type::Point) ||
        !obj->is(Type::Real);
}

void GeometryGenerator::move(const QPointF& delta) {
    static_cast<Point*>(obj.get())->move(delta);

    geom->setChanged();

    recalc();
}

void GeometryGenerator::onChanged() {
    Generator::onChanged();
    item->update();
}
