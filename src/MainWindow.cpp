#include "MainWindow.h"

#include "Function.h"
#include "Scene.h"
#include "EditMode.h"
#include "View.h"

#include <QAction>
#include <QMenuBar>

MainWindow::MainWindow() : QMainWindow() {
    createUi();
    createActionsMenu();

    scene = new Scene(this);
    view->setScene(scene);

    connect(
        scene,
        &Scene::cursorChanged,
        view,
        &View::setCursor
    );
}

void MainWindow::createUi() {
    view = new View(this);
    view->setMouseTracking(true);
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
            scene->setMode(EditMode::MOVE);
        }
    );
    menu->addAction(moveModeAction);

    // Create Point mode
    auto* createPointModeAction = new QAction(tr("Create Point"), this);
    connect(
        createPointModeAction,
        &QAction::triggered,
        [this]() {
            scene->setMode(EditMode::CREATE_POINT);
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
    auto* func = action->data().value<Function*>();

    scene->setMode(EditMode::FUNCTION);
    scene->setFunction(func);
}
