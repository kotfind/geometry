#include "FreeGenerator.h"

#include "Point.h"
#include "GeometryItem.h"
#include "Geometry.h"

#include <QPointF>

FreeGenerator::FreeGenerator(Geometry* geom, Point* pt)
  : Generator(geom),
    origPoint(pt)
{
    object = new Point(*pt);
}

FreeGenerator::~FreeGenerator() {
    delete origPoint;
}

void FreeGenerator::move(const QPointF& delta) {
    item->beginResetObject();
    origPoint->move(delta);
    item->endResetObject();

    recalc();
}

void FreeGenerator::recalcSelf() {
    point()->setPos(geom->transform(origPoint->getPos()));
}

Point* FreeGenerator::point() {
    return static_cast<Point*>(object);
}
