#include "GeometryGenerator.h"

#include "GeometryItem.h"
#include "GeometryObject.h"
#include "Object.h"
#include "Geometry.h"

GeometryItem* GeometryGenerator::getGeometryItem() const {
    return item;
}

bool GeometryGenerator::checkObjectType() const {
    // FIXME
    return obj->is(
        Type::Point |
        Type::Line |
        Type::Circle
    );
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
