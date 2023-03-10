#include "Scene.h"

#include "Function.h"
#include "Point.h"
#include "Generator.h"
#include "GeometryItem.h"

#include <QDebug>

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
}

void Scene::setMode(EditMode m) {
    mode = m;
}

void Scene::setFunction(Function* f) {
    function = f;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    double x = e->scenePos().x();
    double y = e->scenePos().y();

    switch (mode) {
        case EditMode::CREATE_POINT: {
            auto* point = new Point(x, y);
            auto* gen = new Generator(point);
            addItem(gen->getGeometryItem());
        } break;
    }
}
