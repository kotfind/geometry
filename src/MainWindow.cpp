#include "MainWindow.h"

#include "Function.h"

#include <QAction>
#include <QMenuBar>
#include <QGraphicsView>
#include <QGraphicsScene>

MainWindow::MainWindow() : QMainWindow() {
    createUi();
    createActionsMenu();

    scene = new QGraphicsScene(this);
    view->setScene(scene);
}

void MainWindow::createUi() {
    view = new QGraphicsView(this);
    setCentralWidget(view);
}

void MainWindow::createActionsMenu() {
    auto* menu = menuBar()->addMenu(tr("Actions"));

    // Move Mode
    auto* moveModeAction = new QAction(tr("Move"), this);
    connect(
        moveModeAction,
        &QAction::triggered,
        [this]() {
            mode = EditMode::MOVE;
        }
    );
    menu->addAction(moveModeAction);

    // Create Point mode
    auto* createPointModeAction = new QAction(tr("Create Point"), this);
    connect(
        createPointModeAction,
        &QAction::triggered,
        [this]() {
            mode = EditMode::CREATE_POINT;
        }
    );
    menu->addAction(createPointModeAction);

    // Functions
    for (const auto& [name, func] : Function::getAll().asKeyValueRange()) {
        auto* action = new QAction(name, this);
        action->setData(QVariant::fromValue(func));
        connect(
            action,
            &QAction::triggered,
            this,
            &MainWindow::onFunctionActionTriggered
        );
        menu->addAction(action);
    }
}

void MainWindow::onFunctionActionTriggered() {
    auto* action = static_cast<QAction*>(sender());
    currentFunction = action->data().value<Function*>();
    mode = EditMode::FUNCTION;
}
