#include "Scene.h"

#include "Function.h"
#include "Point.h"
#include "Generator.h"
#include "GeometryItem.h"

#include <QDebug>
#include <cassert>
#include <QTransform>

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
}

void Scene::setMode(EditMode m) {
    mode = m;
}

void Scene::setFunction(Function* f) {
    func = f;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    double x = e->scenePos().x();
    double y = e->scenePos().y();

    switch (mode) {
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

            auto* item = static_cast<GeometryItem*>(itemAt(x, y, QTransform()));
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

        default:
            QGraphicsScene::mousePressEvent(e);
    }
}
