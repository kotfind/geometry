#include "Scene.h"

#include "Function.h"
#include "Point.h"
#include "GeometryItem.h"
#include "Geometry.h"
#include "Generator.h"
#include "FreeGenerator.h"
#include "DependantGenerator.h"

#include <QDebug>
#include <cassert>
#include <QTransform>
#include <QPointF>
#include <QCursor>

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
    geom = new Geometry;

    setSceneRect(geom->getSceneRect());
}

Scene::~Scene() {
    delete geom;
}

void Scene::setMode(EditMode m) {
    mode = m;
    selectedFuncArgs.clear();
}

void Scene::setFunction(Function* f) {
    func = f;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();

    updateCursor(e);

    if (!(e->buttons() & Qt::LeftButton)) return;

    switch (mode) {
        case EditMode::MOVE:
        {
            currentFreeGenerator = getFreeGeneratorAt(pos);
        }
        break;

        case EditMode::CREATE_POINT:
        {
            auto* point = new Point(pos);
            auto* gen = new FreeGenerator(geom, point);
            auto* item = gen->getGeometryItem();
            addItem(item);
        }
        break;

        case EditMode::FUNCTION:
        {
            assert(func);

            auto* gen = getGeneratorAt(
                pos,
                func->getTypeHints()[selectedFuncArgs.size()]
            );
            if (!gen) break;

            selectedFuncArgs << gen;
            if (selectedFuncArgs.size() == func->countArgs()) {
                for (int funcResNum = 0; funcResNum < func->getMaxReturnSize(); ++funcResNum) {
                    auto* gen = new DependantGenerator(geom, func, selectedFuncArgs, funcResNum);
                    auto* item = gen->getGeometryItem();
                    addItem(item);
                }

                selectedFuncArgs.clear();
            }
        }
        break;

        case EditMode::REMOVE:
        {
            auto* gen = getGeneratorAt(pos);
            if (gen) {
                gen->remove();
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

    switch (mode) {
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

    switch (mode) {
        case EditMode::MOVE:
        {
            currentFreeGenerator = nullptr;
        }
        break;
    }
}

FreeGenerator* Scene::getFreeGeneratorAt(const QPointF& pos) const {
    auto itemList = items(pos);
    for (auto* item_ : itemList) {
        auto* item = static_cast<GeometryItem*>(item_);
        auto* gen = item->getGenerator();
        if (gen->isFree()) {
            return static_cast<FreeGenerator*>(gen);
        }
    }
    return nullptr;
}

Generator* Scene::getGeneratorAt(const QPointF& pos, int type) const {
    auto itemList = items(pos);
    for (auto* item_ : itemList) {
        auto* item = static_cast<GeometryItem*>(item_);
        auto* gen = item->getGenerator();
        if (type == 0 || gen->getObjectType() == type) {
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
            if (getGeneratorAt(
                pos,
                func->getTypeHints()[selectedFuncArgs.size()]
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

