#include "GeometryItem.h"

#include "GeometryObject.h"
#include "Generator.h"

#include <QGraphicsScene>

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
    if (object()) {
        prepareGeometryChange();
    }
}

void GeometryItem::paint(QPainter* qp, const QStyleOptionGraphicsItem*, QWidget*) {
    if (object()) {
        object()->paint(qp);
    }
}

QRectF GeometryItem::boundingRect() const {
    return object() ? object()->boundingRect() : QRectF();
}

QPainterPath GeometryItem::shape() const {
    return object() ? object()->shape() : QPainterPath();
}

const GeometryObject* GeometryItem::object() const {
    return dynamic_cast<const GeometryObject*>(gen->getObject());
}

void GeometryItem::remove() {
    if (scene()) {
        scene()->removeItem(this);
    }
    delete this;
}
