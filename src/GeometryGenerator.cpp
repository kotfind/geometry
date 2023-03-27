#include "GeometryGenerator.h"

#include "GeometryItem.h"

GeometryGenerator::GeometryGenerator(GeometryObject* obj, Geometry* geom)
  : Generator(geom),
    obj(obj)
{
    item = new GeometryItem(this);
}

void GeometryGenerator::beginResetObject() {
    item->beginResetObject();
}

void GeometryGenerator::endResetObject() {
    item->endResetObject();
}
