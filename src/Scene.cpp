#include "Scene.h"

#include "Function.h"
#include "Point.h"
#include "Generator.h"
#include "GeometryItem.h"

#include <QDebug>
#include <cassert>
#include <QTransform>
#include <QPointF>

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
}

void Scene::setMode(EditMode m) {
    mode = m;
}

void Scene::setFunction(Function* f) {
    func = f;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    QPointF pos = e->scenePos();
    double x = pos.x();
    double y = pos.y();

    switch (mode) {
        case EditMode::MOVE:
        {
            currentFreeGenerator = getFreeGeneratorAt(pos);
        }
        break;

        case EditMode::CREATE_POINT:
        {
            auto* point = new Point(x, y);
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
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    QPointF pos = e->scenePos();
    double x = pos.x();
    double y = pos.y();

    switch (mode) {
        case EditMode::MOVE:
        {
            if (!currentFreeGenerator) break;
            currentFreeGenerator->setPos(x, y);
        }
        break;
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    QPointF pos = e->scenePos();
    double x = pos.x();
    double y = pos.y();

    switch (mode) {
        case EditMode::MOVE:
        {
            currentFreeGenerator = nullptr;
        }
        break;
    }
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
