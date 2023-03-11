#include "GeometryItem.h"

#include "GeometryObject.h"
#include "Generator.h"

#include <QRectF>
#include <QPainter>

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
        update(getObject()->boundingRect());
    }
}

void GeometryItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (getObject()) {
        getObject()->paint(painter);
    }
}

QRectF GeometryItem::boundingRect() const {
    return getObject() ? getObject()->boundingRect() : QRectF();
}

GeometryObject* GeometryItem::getObject() const {
    return dynamic_cast<GeometryObject*>(gen->object);
}
