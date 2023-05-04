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

    switch (geom->getEditMode()) {
        case EditMode::MOVE:
        {
            currentFreeGenerator = getFreeGeneratorAt(pos);
        }
        break;

        case EditMode::CREATE_POINT:
        {
            auto point = std::make_unique<Point>(pos);
            auto* gen = geom->makeGeometryGenerator(std::move(point));
            auto* item = gen->getGeometryItem();
            addItem(item);
        }
        break;

        case EditMode::FUNCTION:
        {
            auto* gen = getGeneratorAt(
                pos,
                geom->getNextFuncArgType()
            );
            if (!gen) break;

            geom->selectFuncArg(gen, this);
        }
        break;

        case EditMode::REMOVE:
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

    switch (geom->getEditMode()) {
        case EditMode::MOVE:
        {
            if (!currentFreeGenerator) break;
            currentFreeGenerator->move(delta);
        }
        break;
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    if (!(e->buttons() & Qt::LeftButton)) return;

    switch (geom->getEditMode()) {
        case EditMode::MOVE:
        {
            currentFreeGenerator = nullptr;
        }
        break;
    }
}

GeometryGenerator* Scene::getFreeGeneratorAt(const QPointF& pos) const {
    auto itemList = items(pos);
    for (auto* item_ : itemList) {
        auto* item = static_cast<GeometryItem*>(item_);
        auto* gen = item->getGeometryGenerator();
        if (gen->isFree()) {
            return gen;
        }
    }
    return nullptr;
}

GeometryGenerator* Scene::getGeneratorAt(const QPointF& pos, Type type) const {
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

    switch (geom->getEditMode()) {
        case EditMode::MOVE:
            if (currentFreeGenerator && (e->buttons() & Qt::LeftButton)) {
                // Moving item
                emit cursorChanged(Qt::ClosedHandCursor);
                return;
            }
            if (getFreeGeneratorAt(pos)) {
                // Can move item
                emit cursorChanged(Qt::OpenHandCursor);
                return;
            }
            break;

        case EditMode::CREATE_POINT:
            emit cursorChanged(Qt::CrossCursor);
            return;

        case EditMode::FUNCTION:
            if (getGeneratorAt(
                pos,
                geom->getNextFuncArgType()
            )) {
                emit cursorChanged(Qt::PointingHandCursor);
                return;
            }
            break;

        case EditMode::REMOVE:
            if (getGeneratorAt(pos)) {
                emit cursorChanged(Qt::ForbiddenCursor);
                return;
            }
            break;
    }

    emit cursorChanged({});
}

