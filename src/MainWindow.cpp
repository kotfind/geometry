#include "MainWindow.h"

#include "Function.h"

#include <QAction>
#include <QMenuBar>

MainWindow::MainWindow() : QMainWindow() {
    createUi();
    createFunctionActions();
}

void MainWindow::createUi() {

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
