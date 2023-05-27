#include "GeometryItem.h"

#include "GeometryObject.h"
#include "GeometryGenerator.h"
#include "Engine.h"
#include "EditMode.h"

#include <QGraphicsScene>

GeometryItem::GeometryItem(GeometryGenerator* gen)
: QGraphicsItem(),
  gen(gen)
{}

void GeometryItem::paint(QPainter* qp, const QStyleOptionGraphicsItem*, QWidget*) {
    bool showHidden = gen->getEngine()->getEditMode()->getType() == EditMode::Type::HIDE;
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

    if (auto genObj = gen->getGeometryObject()) {
        obj.reset(
            static_cast<GeometryObject*>(
                genObj->clone()
            )
        );

        obj->transform(
            gen->getEngine()->getActiveGeometry()->getTransformation()
        );
    } else {
        obj.reset(nullptr);
    }
}

bool GeometryItem::isHidden() const {
    return hidden;
}

void GeometryItem::setHidden(bool v) {
    hidden = v;
    QGraphicsItem::update();
    if (auto engine = gen->getEngine(); engine) {
        engine->setChanged();
    }
}

void GeometryItem::toggleHidden() {
    hidden = !hidden;
    QGraphicsItem::update();
    if (auto engine = gen->getEngine()) {
        engine->setChanged();
    }
}
