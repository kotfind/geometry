#include "GeometryItem.h"

#include "GeometryObject.h"
#include "GeometryGenerator.h"
#include "Geometry.h"
#include "EditMode.h"

#include <QGraphicsScene>

GeometryItem::GeometryItem(GeometryGenerator* gen)
: QGraphicsItem(),
  gen(gen)
{}

void GeometryItem::paint(QPainter* qp, const QStyleOptionGraphicsItem*, QWidget*) {
    bool showHidden = gen->getGeometry()->getEditMode()->getType() == EditMode::Type::HIDE;
    if (obj && (!hidden || showHidden)) {
        obj->paint(qp, QColor(0, 0, 0, hidden ? 127 : 255));
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

bool GeometryItem::isHidden() const {
    return hidden;
}

void GeometryItem::setHidden(bool v) {
    hidden = v;
    update();
}

void GeometryItem::toggleHidden() {
    hidden = !hidden;
    update();
}
