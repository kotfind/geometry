#include "Scene.h"

#include "Function.h"
#include "Point.h"
#include "GeometryItem.h"
#include "Engine.h"
#include "Generator.h"
#include "GeometryGenerator.h"
#include "EditMode.h"

#include <QDebug>
#include <cassert>
#include <QTransform>
#include <QPointF>
#include <QCursor>

Scene::Scene(Engine* engine, QObject* parent)
  : QGraphicsScene(parent),
    engine(engine)
{
    setSceneRect(engine->getSceneRect());
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    updateCursor(e);

    if (!(e->buttons() & Qt::LeftButton)) return;

    switch (engine->getEditMode()->getType()) {
        case EditMode::Type::MOVE:
        {
            currentFreeGenerator = getFreeOrRestrictedGeneratorAt(pos);
        }
        break;

        case EditMode::Type::CREATE_POINT:
        {
            GeometryGenerator* gen;
            if (auto* restrictor = getDependantGeneratorAt(pos)) {
                // Make Restrcted Generator
                gen = engine->makeGeometryGenerator(restrictor);
                gen->setPos(pos);
            } else {
                // Make Free Generator
                auto point = std::make_unique<Point>(
                    engine->getGeometry()->getTransformation()->untransform(pos)
                );
                gen = engine->makeGeometryGenerator(std::move(point));
            }
            auto* item = gen->getGeometryItem();
            addItem(item);
        }
        break;

        case EditMode::Type::FUNCTION:
        {
            auto* gen = getGeneratorAt(
                pos,
                engine->getNextFuncArgType()
            );
            if (!gen) break;

            engine->selectFuncArg(gen, this);
        }
        break;

        case EditMode::Type::REMOVE:
        {
            auto* gen = getGeneratorAt(pos);
            if (gen) {
                engine->removeGenerator(gen);
            }
        }
        break;

        case EditMode::Type::HIDE:
        {
            auto* gen = getGeneratorAt(pos, Object::Type::Any, true);

            if (gen) {
                gen->getGeometryItem()->toggleHidden();
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
        engine->move(delta);
        update();
    }

    if (!(e->buttons() & Qt::LeftButton)) return;

    switch (engine->getEditMode()->getType()) {
        case EditMode::Type::MOVE:
        {
            if (!currentFreeGenerator) break;
            currentFreeGenerator->setPos(
                engine->getGeometry()->getTransformation()->untransform(pos)
            );
        }
        break;
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    if (!(e->buttons() & Qt::LeftButton)) return;

    switch (engine->getEditMode()->getType()) {
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
        if (item->isHidden()) continue;

        auto* gen = item->getGeometryGenerator();
        if (gen->isFree() || gen->isRestricted()) {
            return gen;
        }
    }
    return nullptr;
}

GeometryGenerator* Scene::getDependantGeneratorAt(const QPointF& pos) const {
    auto itemList = items(pos);
    for (auto* item_ : itemList) {
        auto* item = static_cast<GeometryItem*>(item_);
        if (item->isHidden()) continue;

        auto* gen = item->getGeometryGenerator();
        if (gen->isDependant()) {
            return gen;
        }
    }
    return nullptr;
}

GeometryGenerator* Scene::getGeneratorAt(const QPointF& pos, Object::Type type, bool allowHidden) const {
    auto itemList = items(pos);
    for (auto* item_ : itemList) {
        auto* item = static_cast<GeometryItem*>(item_);
        if (item->isHidden() && !allowHidden) continue;

        auto* gen = item->getGeometryGenerator();
        if (gen->getObject()->is(type)) {
            return gen;
        }
    }
    return nullptr;
}

void Scene::updateCursor(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    switch (engine->getEditMode()->getType()) {
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
            emit cursorChanged(getDependantGeneratorAt(pos)
                ? Qt::PointingHandCursor
                : Qt::CrossCursor
            );
            return;

        case EditMode::Type::FUNCTION:
            if (getGeneratorAt(
                pos,
                engine->getNextFuncArgType()
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

        case EditMode::Type::HIDE:
            if (getGeneratorAt(pos, Object::Type::Any, true)) {
                emit cursorChanged(Qt::PointingHandCursor);
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
        engine->zoom(ang, e->scenePos());
    } else {
        // Scroll
        engine->scroll(
            e->orientation() == Qt::Horizontal
                ? QPointF(ang, 0)
                : QPointF(0, ang)
        );
    }
}
