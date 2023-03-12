#include "Scene.h"

#include "Function.h"
#include "Point.h"
#include "Generator.h"
#include "GeometryItem.h"

#include <QDebug>
#include <cassert>
#include <QTransform>
#include <QPointF>
#include <QCursor>
#include <QApplication>

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
}

void Scene::setMode(EditMode m) {
    mode = m;
}

void Scene::setFunction(Function* f) {
    func = f;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    switch (mode) {
        case EditMode::MOVE:
        {
            currentFreeGenerator = getFreeGeneratorAt(pos);
        }
        break;

        case EditMode::CREATE_POINT:
        {
            auto* point = new Point(pos.x(), pos.y());
            auto* gen = new Generator(point);
            auto* item = gen->getGeometryItem();
            addItem(item);
        }
        break;

        case EditMode::FUNCTION:
        {
            // TODO check type hints
            assert(func);

            auto* item = static_cast<GeometryItem*>(itemAt(pos, QTransform()));
            if (!item) break;

            auto* gen = item->getGenerator();
            selectedFuncArgs << gen;
            if (selectedFuncArgs.size() == func->countArgs()) {
                auto* gen = new Generator(func, selectedFuncArgs, 0 /* XXX */);
                auto* item = gen->getGeometryItem();
                addItem(item);

                selectedFuncArgs.clear();
            }
        }
        break;
    }

    updateCursor(e);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    switch (mode) {
        case EditMode::MOVE:
        {
            if (!currentFreeGenerator) break;
            currentFreeGenerator->setPos(pos.x(), pos.y());
        }
        break;
    }

    updateCursor(e);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    switch (mode) {
        case EditMode::MOVE:
        {
            currentFreeGenerator = nullptr;
        }
        break;
    }

    updateCursor(e);
}

Generator* Scene::getFreeGeneratorAt(const QPointF& pos) const {
    auto itemList = items(pos);
    for (auto* item_ : itemList) {
        auto* item = static_cast<GeometryItem*>(item_);
        auto* gen = item->getGenerator();
        if (gen->isFree()) {
            return gen;
        }
    }
    return nullptr;
}

void Scene::updateCursor(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    switch (mode) {
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
            if (itemAt(pos, QTransform())) {
                emit cursorChanged(Qt::PointingHandCursor);
                return;
            }
            break;
    }

    emit cursorChanged({});
}
