#include "GeometryItem.h"

#include "GeometryObject.h"
#include "Generator.h"

GeometryItem::GeometryItem(Generator* gen)
: QGraphicsItem(),
  gen(gen)
{
}

void GeometryItem::beginResetObject() {
    updateBoundingRect();
}

void GeometryItem::endResetObject() {
    updateBoundingRect();
}

void GeometryItem::updateBoundingRect() {
    if (getObject()) {
        prepareGeometryChange();
        // update(getObject()->boundingRect());
    }
}

void GeometryItem::paint(QPainter* qp, const QStyleOptionGraphicsItem*, QWidget*) {
    if (getObject()) {
        getObject()->paint(qp);
    }
}

QRectF GeometryItem::boundingRect() const {
    return getObject() ? getObject()->boundingRect() : QRectF();
}

QPainterPath GeometryItem::shape() const {
    return getObject() ? getObject()->shape() : QPainterPath();
}

GeometryObject* GeometryItem::getObject() const {
    return dynamic_cast<GeometryObject*>(gen->object);
}
