#include "Scene.h"

#include "Function.h"
#include "AbstractPoint.h"
#include "GeometryItem.h"
#include "Engine.h"
#include "Generator.h"
#include "GeometryGenerator.h"
#include "EditMode.h"
#include "AbstractTransformation.h"

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

void Scene::detachAll() {
    while (!items().isEmpty()) {
        auto* item = items()[0];
        removeItem(item);
    }
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
            auto pt = std::unique_ptr<AbstractPoint>(
                engine->makeUntransformedPoint(pos)
            );
            if (!pt) return;

            if (auto* restrictor = getDependantGeneratorAt(pos)) {
                // Make Restrcted Generator
                gen = engine->makeGeometryGenerator(
                    engine->getActiveGeometry(),
                    restrictor
                );
                gen->setPos(pt.get());
            } else {
                // Make Free Generator
                gen = engine->makeGeometryGenerator(
                    engine->getActiveGeometry(),
                    std::move(pt)
                );
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
            auto* gen = getGeneratorAt(pos, ~0, true);

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
        auto from = std::unique_ptr<AbstractPoint>(
            engine->makeUntransformedPoint(e->lastScenePos())
        );
        if (!from) return;

        auto to = std::unique_ptr<AbstractPoint>(
            engine->makeUntransformedPoint(e->scenePos())
        );
        if (!to) return;

        engine->move(from.get(), to.get());
        update();
    }

    if (!(e->buttons() & Qt::LeftButton)) return;

    switch (engine->getEditMode()->getType()) {
        case EditMode::Type::MOVE:
        {
            if (!currentFreeGenerator) break;
            if (auto pt = std::unique_ptr<AbstractPoint>(engine->makeUntransformedPoint(pos))) {
                currentFreeGenerator->setPos(pt.get());
            }
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
    return getGeneratorAtHelper(pos, true, true, false, ~0, false);
}

GeometryGenerator* Scene::getDependantGeneratorAt(const QPointF& pos) const {
    return getGeneratorAtHelper(pos, false, false, true, ~0, false);
}

GeometryGenerator* Scene::getGeneratorAt(const QPointF& pos, int type, bool allowHidden) const {
    return getGeneratorAtHelper(pos, true, true, true, type, allowHidden);
}

GeometryGenerator* Scene::getGeneratorAtHelper(
    const QPointF& pos,
    bool allowFree,
    bool allowRestricted,
    bool allowDependant,
    int objectType,
    bool allowHidden
) const {
    auto itemList = items(pos);
    for (auto* item_ : itemList) {
        if (item_ == engine->getActiveGeometry()->getGraphicsItem()) continue;

        auto* item = static_cast<GeometryItem*>(item_);
        if (item->isHidden() && !allowHidden) continue;

        auto* gen = item->getGeometryGenerator();
        if (gen->isFree() && !allowFree) continue;
        if (gen->isRestricted() && !allowRestricted) continue;
        if (gen->isDependant() && !allowDependant) continue;
        if (!gen->getObject()->is(objectType)) continue;

        return gen;
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
            if (getGeneratorAt(pos, ~0, true)) {
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
