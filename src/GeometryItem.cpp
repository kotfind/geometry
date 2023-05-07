#include "GeometryItem.h"

#include "GeometryObject.h"
#include "GeometryGenerator.h"
#include "Geometry.h"

#include <QGraphicsScene>

GeometryItem::GeometryItem(GeometryGenerator* gen)
: QGraphicsItem(),
  gen(gen)
{}

void GeometryItem::paint(QPainter* qp, const QStyleOptionGraphicsItem*, QWidget*) {
    if (obj) {
        obj->paint(qp);
    }
}

QRectF GeometryItem::boundingRect() const {
    return obj ? obj->boundingRect() : QRectF();
}

QPainterPath GeometryItem::shape() const {
    return obj ? obj->shape() : QPainterPath();
}

void GeometryItem::remove() {
    if (scene()) {
        scene()->removeItem(this);
    }
    delete this;
}

void GeometryItem::update() {
    prepareGeometryChange();

    auto* object = gen->getGeometryObject();
    obj.reset(
        object
        ? object->transformed(
                gen->getGeometry()->getTransformation()
            )
        : nullptr
    );
}
