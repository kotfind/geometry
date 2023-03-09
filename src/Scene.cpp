#include "Scene.h"

#include "Function.h"

#include <QDebug>

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
}

void Scene::setMode(EditMode m) {
    mode = m;
}

void Scene::setFunction(Function* f) {
    function = f;
}
