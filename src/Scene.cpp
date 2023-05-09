#include "Scene.h"

#include "Function.h"
#include "Point.h"
#include "GeometryItem.h"
#include "Geometry.h"
#include "Generator.h"
#include "GeometryGenerator.h"
#include "EditMode.h"

#include <QDebug>
#include <cassert>
#include <QTransform>
#include <QPointF>
#include <QCursor>

Scene::Scene(Geometry* geom, QObject* parent)
  : QGraphicsScene(parent),
    geom(geom)
{
    setSceneRect(geom->getSceneRect());
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    updateCursor(e);

    if (!(e->buttons() & Qt::LeftButton)) return;

    switch (geom->getEditMode()->getType()) {
        case EditMode::Type::MOVE:
        {
            currentFreeGenerator = getFreeOrRestrictedGeneratorAt(pos);
        }
        break;

        case EditMode::Type::CREATE_POINT:
        {
            auto point = std::unique_ptr<Point>(
                std::make_unique<Point>(pos)->untransformed(
                    geom->getTransformation()
                )
            );
            auto* gen = geom->makeGeometryGenerator(std::move(point));
            auto* item = gen->getGeometryItem();
            addItem(item);
        }
        break;

        case EditMode::Type::FUNCTION:
        {
            auto* gen = getGeneratorAt(
                pos,
                geom->getNextFuncArgType()
            );
            if (!gen) break;

            geom->selectFuncArg(gen, this);
        }
        break;

        case EditMode::Type::REMOVE:
        {
            auto* gen = getGeneratorAt(pos);
            if (gen) {
                geom->removeGenerator(gen);
            }
        }
        break;
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();
    auto delta = e->scenePos() - e->lastScenePos();

    updateCursor(e);

    if (e->buttons() & Qt::MiddleButton) {
        geom->move(delta);
        update();
    }

    if (!(e->buttons() & Qt::LeftButton)) return;

    switch (geom->getEditMode()->getType()) {
        case EditMode::Type::MOVE:
        {
            if (!currentFreeGenerator) break;
            currentFreeGenerator->setPos(
                std::unique_ptr<Point>(
                    std::make_unique<Point>(pos)->untransformed(
                        geom->getTransformation()
                    )
                )->getPos()
            );
        }
        break;
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    if (!(e->buttons() & Qt::LeftButton)) return;

    switch (geom->getEditMode()->getType()) {
        case EditMode::Type::MOVE:
        {
            currentFreeGenerator = nullptr;
        }
        break;
    }
}

GeometryGenerator* Scene::getFreeOrRestrictedGeneratorAt(const QPointF& pos) const {
    auto itemList = items(pos);
    for (auto* item_ : itemList) {
        auto* item = static_cast<GeometryItem*>(item_);
        auto* gen = item->getGeometryGenerator();
        if (gen->isFree() || gen->isRestricted()) {
            return gen;
        }
    }
    return nullptr;
}

GeometryGenerator* Scene::getGeneratorAt(const QPointF& pos, Object::Type type) const {
    auto itemList = items(pos);
    for (auto* item_ : itemList) {
        auto* item = static_cast<GeometryItem*>(item_);
        auto* gen = item->getGeometryGenerator();
        if (gen->getObject()->is(type)) {
            return gen;
        }
    }
    return nullptr;
}

void Scene::updateCursor(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    switch (geom->getEditMode()->getType()) {
        case EditMode::Type::MOVE:
            if (currentFreeGenerator && (e->buttons() & Qt::LeftButton)) {
                // Moving item
                emit cursorChanged(Qt::ClosedHandCursor);
                return;
            }
            if (getFreeOrRestrictedGeneratorAt(pos)) {
                // Can move item
                emit cursorChanged(Qt::OpenHandCursor);
                return;
            }
            break;

        case EditMode::Type::CREATE_POINT:
            emit cursorChanged(Qt::CrossCursor);
            return;

        case EditMode::Type::FUNCTION:
            if (getGeneratorAt(
                pos,
                geom->getNextFuncArgType()
            )) {
                emit cursorChanged(Qt::PointingHandCursor);
                return;
            }
            break;

        case EditMode::Type::REMOVE:
            if (getGeneratorAt(pos)) {
                emit cursorChanged(Qt::ForbiddenCursor);
                return;
            }
            break;
    }

    emit cursorChanged({});
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent* e) {
    double ang = e->delta() / 8.;

    if (e->modifiers() & Qt::ControlModifier) {
        // Zoom
        geom->zoom(ang, e->scenePos());
    } else {
        // Scroll
        geom->scroll(
            e->orientation() == Qt::Horizontal
                ? QPointF(ang, 0)
                : QPointF(0, ang)
        );
    }
}
