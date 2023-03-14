#include "Scene.h"

#include "Function.h"
#include "Point.h"
#include "Generator.h"
#include "GeometryItem.h"
#include "Geometry.h"

#include <QDebug>
#include <cassert>
#include <QTransform>
#include <QPointF>
#include <QCursor>
#include <QApplication>

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
    geometry = new Geometry;

    setSceneRect(geometry->getSceneRect());
}

Scene::~Scene() {
    delete geometry;
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
            geometry->addGenerator(gen);
            auto* item = gen->getGeometryItem();
            addItem(item);
        }
        break;

        case EditMode::FUNCTION:
        {
            assert(func);

            auto* gen = getTypedGeneratorAt(
                pos,
                func->getTypeHints()[selectedFuncArgs.size()]
            );
            if (!gen) break;

            selectedFuncArgs << gen;
            if (selectedFuncArgs.size() == func->countArgs()) {
                for (int funcResNum = 0; funcResNum < func->getMaxReturnSize(); ++funcResNum) {
                    auto* gen = new Generator(func, selectedFuncArgs, funcResNum);
                    geometry->addGenerator(gen);
                    auto* item = gen->getGeometryItem();
                    addItem(item);
                }

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

Generator* Scene::getTypedGeneratorAt(const QPointF& pos, int type) const {
    auto itemList = items(pos);
    for (auto* item_ : itemList) {
        auto* item = static_cast<GeometryItem*>(item_);
        auto* gen = item->getGenerator();
        if (gen->getObjectType() == type) {
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

            if (getTypedGeneratorAt(
                pos,
                func->getTypeHints()[selectedFuncArgs.size()]
            )) {
                emit cursorChanged(Qt::PointingHandCursor);
                return;
            }
            break;
    }

    emit cursorChanged({});
}
