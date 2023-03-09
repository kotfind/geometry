#include "MainWindow.h"

#include "Function.h"

#include <QAction>
#include <QMenuBar>
#include <QGraphicsView>
#include <QGraphicsScene>

MainWindow::MainWindow() : QMainWindow() {
    createUi();
    createFunctionActions();

    scene = new QGraphicsScene(this);
    view->setScene(scene);
}

void MainWindow::createUi() {
    view = new QGraphicsView(this);
    setCentralWidget(view);
}

void MainWindow::createFunctionActions() {
    auto* menu = menuBar()->addMenu("Functions");
    for (const auto& [name, func] : Function::getAll().asKeyValueRange()) {
        auto* action = new QAction(name, this);
        action->setData(QVariant::fromValue(func));
        connect(
            action,
            &QAction::triggered,
            this,
            &MainWindow::functionActionTriggered
        );
        menu->addAction(action);
    }
}

void MainWindow::functionActionTriggered() {
    auto* action = static_cast<QAction*>(sender());
    currentFunction = action->data().value<Function*>();
}
