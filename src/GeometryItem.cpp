#include "GeometryItem.h"

#include "GeometryObject.h"

#include <QRectF>
#include <QPainter>

GeometryItem::GeometryItem() : QGraphicsItem() {
}

void GeometryItem::setObject(GeometryObject* o) {
    if (o == object) return;
    if (object) {
        update(object->boundingRect());
    }
    object = o;
    if (object) {
        update(object->boundingRect());
    }
}

void GeometryItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (object) {
        object->paint(painter);
    }
}

QRectF GeometryItem::boundingRect() const {
    return object ? object->boundingRect() : QRectF();
}
