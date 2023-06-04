#include "View.h"

View::View(QWidget* parent) : QGraphicsView(parent) {
}

void View::resizeEvent(QResizeEvent*) {
    if (!scene()) return;
    fitInView(
        scene()->sceneRect(),
        Qt::KeepAspectRatio
    );
    scene()->update();
}
