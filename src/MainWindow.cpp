#include "MainWindow.h"

#include "Function.h"
#include "Scene.h"
#include "EditMode.h"
#include "View.h"
#include "Geometry.h"
#include "IOError.h"

#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>

MainWindow::MainWindow() : QMainWindow() {
    updateTitle();

    createUi();
    createFileMenu();
    createActionsMenu();

    scene = new Scene(this);
    view->setScene(scene);

    connect(
        scene,
        &Scene::cursorChanged,
        view,
        &View::setCursor
    );

    connect(
        newAction,
        &QAction::triggered,
        this,
        &MainWindow::onNewActionTriggered
    );

    connect(
        saveAction,
        &QAction::triggered,
        this,
        &MainWindow::onSaveActionTriggered
    );

    connect(
        saveAsAction,
        &QAction::triggered,
        this,
        &MainWindow::onSaveAsActionTriggered
    );

    connect(
        openAction,
        &QAction::triggered,
        this,
        &MainWindow::onOpenActionTriggered
    );
}

void MainWindow::createUi() {
    view = new View(this);
    view->setMouseTracking(true);
    setCentralWidget(view);
}

void MainWindow::createFileMenu() {
    auto* menu = menuBar()->addMenu(tr("File"));

    newAction = new QAction(tr("New"), this);
    menu->addAction(newAction);

    saveAction = new QAction(tr("Save"), this);
    menu->addAction(saveAction);

    saveAsAction = new QAction(tr("Save As"), this);
    menu->addAction(saveAsAction);

    openAction = new QAction(tr("Open"), this);
    menu->addAction(openAction);
}

void MainWindow::createActionsMenu() {
    auto* menu = menuBar()->addMenu(tr("Actions"));

    menu->addAction(createModeAction(
        tr("Move"),
        EditMode::MOVE
    ));

    menu->addAction(createModeAction(
        tr("Create Point"),
        EditMode::CREATE_POINT
    ));

    menu->addAction(createModeAction(
        tr("Remove"),
        EditMode::REMOVE
    ));

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

QAction* MainWindow::createModeAction(const QString& name, EditMode mode) {
    auto* action = new QAction(name, this);
    connect(
        action,
        &QAction::triggered,
        [this, mode]() {
            scene->setMode(mode);
        }
    );
    return action;
}

void MainWindow::onFunctionActionTriggered() {
    auto* action = static_cast<QAction*>(sender());
    auto* func = action->data().value<Function*>();

    scene->setMode(EditMode::FUNCTION);
    scene->setFunction(func);
}

void MainWindow::onNewActionTriggered() {
    auto* geom = scene->getGeometry();

    if (geom->hasGenerators()) {
        auto reply = QMessageBox::question(
            this,
            tr("Save file?"),
            tr("Would you like to save oppened file?")
        );
        if (reply == QMessageBox::Yes) {
            onSaveActionTriggered();
        }

        geom->clear();
    }

    updateTitle();
}

void MainWindow::onSaveActionTriggered() {
    if (openedFileName.isEmpty()) {
        onSaveAsActionTriggered();
        return;
    }

    try {
        scene->getGeometry()->save(openedFileName);
        updateTitle();
    } catch (const IOError& err) {
        QMessageBox::critical(
            this,
            tr("Input/ Output error"),
            err.what()
        );
    }
}

void MainWindow::onSaveAsActionTriggered() {
    auto fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save file")
    );

    if (fileName.isEmpty()) return;

    try {
        scene->getGeometry()->save(fileName);
        openedFileName = fileName;
        updateTitle();
    } catch (const IOError& err) {
        QMessageBox::critical(
            this,
            tr("Input/ Output error"),
            err.what()
        );
    }
}

void MainWindow::onOpenActionTriggered() {
    auto* geom = scene->getGeometry();

    onNewActionTriggered();

    auto fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open file")
    );

    if (fileName.isEmpty()) return;

    try {
        geom->load(fileName);
        geom->populateScene(scene);
        openedFileName = fileName;

        updateTitle();
    } catch (const IOError& err) {
        QMessageBox::critical(
            this,
            tr("Input/ Output error"),
            err.what()
        );
    }
}

void MainWindow::updateTitle() {
    setWindowTitle(openedFileName.isEmpty()
        ? tr("[Untitled]")
        : QFileInfo(openedFileName).fileName()
    );
}
