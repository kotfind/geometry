#include "MainWindow.h"

#include "Function.h"
#include "Scene.h"
#include "EditMode.h"
#include "View.h"
#include "Geometry.h"
#include "IOError.h"
#include "ToolInfoWidget.h"
#include "Section.h"

#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <QCloseEvent>
#include <QDockWidget>
#include <QHash>

MainWindow::MainWindow() : QMainWindow() {
    updateTitle();

    createUi();
    createFileMenu();
    createToolsMenu();
    createDocks();

    scene = new Scene(this);
    view->setScene(scene);

    toolInfoWidget->setMode(EditMode::MOVE);

    connect(
        scene,
        &Scene::cursorChanged,
        view,
        &View::setCursor
    );

    connect(
        scene,
        &Scene::selectedCountChanged,
        toolInfoWidget,
        &ToolInfoWidget::updateSelectedCount
    );
}

void MainWindow::createUi() {
    view = new View(this);
    view->setMouseTracking(true);
    setCentralWidget(view);
}

void MainWindow::createFileMenu() {
    auto* menu = menuBar()->addMenu(tr("File"));

    auto* newAction = new QAction(tr("New"), this);
    newAction->setShortcut(Qt::CTRL | Qt::Key_N);
    connect(
        newAction,
        &QAction::triggered,
        this,
        &MainWindow::onNewActionTriggered
    );
    menu->addAction(newAction);

    auto* saveAction = new QAction(tr("Save"), this);
    saveAction->setShortcut(Qt::CTRL | Qt::Key_S);
    connect(
        saveAction,
        &QAction::triggered,
        this,
        &MainWindow::onSaveActionTriggered
    );
    menu->addAction(saveAction);

    auto* saveAsAction = new QAction(tr("Save As"), this);
    saveAsAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S);
    connect(
        saveAsAction,
        &QAction::triggered,
        this,
        &MainWindow::onSaveAsActionTriggered
    );
    menu->addAction(saveAsAction);

    auto* openAction = new QAction(tr("Open"), this);
    openAction->setShortcut(Qt::CTRL | Qt::Key_O);
    connect(
        openAction,
        &QAction::triggered,
        this,
        &MainWindow::onOpenActionTriggered
    );
    menu->addAction(openAction);
}

void MainWindow::createToolsMenu() {
    menuBar()->addAction(new QAction("|", this)); // Separator
    auto* menu = menuBar()->addMenu(tr("Tools"));

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

    for (auto* sec : Section::getMaster()->getSections()) {
        menuBar()->addMenu(getSectionMenu(sec));
    }

    menuBar()->addAction(new QAction("|", this)); // Separator
}

QMenu* MainWindow::getSectionMenu(Section* section) {
    auto* menu = new QMenu(section->getName(), this);
    for (auto* sec : section->getSections()) {
        menu->addMenu(getSectionMenu(sec));
    }
    for (auto* func : section->getFunctions()) {
        auto* action = new QAction(func->getSelfName(), this);
        action->setData(QVariant::fromValue(func));
        connect(
            action,
            &QAction::triggered,
            this,
            &MainWindow::onFunctionActionTriggered
        );
        menu->addAction(action);
    }
    return menu;
}

void MainWindow::createDocks() {
    toolInfoWidget = new ToolInfoWidget(this);
    createDock(toolInfoWidget, tr("Tool Info"));
}

void MainWindow::createDock(QWidget* widget, const QString& name) {
    auto* dock = new QDockWidget(name, this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(widget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    menuBar()->addMenu(tr("View"))->addAction(dock->toggleViewAction());
}

QAction* MainWindow::createModeAction(const QString& name, EditMode mode) {
    auto* action = new QAction(name, this);
    action->setData(QVariant::fromValue(mode));
    connect(
        action,
        &QAction::triggered,
        this,
        &MainWindow::onModeActionTriggered
    );
    return action;
}

void MainWindow::onFunctionActionTriggered() {
    auto* action = static_cast<QAction*>(sender());
    auto* func = action->data().value<Function*>();

    scene->setMode(EditMode::FUNCTION);
    scene->setFunction(func);
    toolInfoWidget->setMode(EditMode::FUNCTION);
    toolInfoWidget->setFunction(func);
}

void MainWindow::onModeActionTriggered() {
    auto* action = static_cast<QAction*>(sender());
    auto mode = action->data().value<EditMode>();

    scene->setMode(mode);
    toolInfoWidget->setMode(mode);
}

void MainWindow::askForSave() {
    if (scene->getGeometry()->isChanged()) {
        auto reply = QMessageBox::question(
            this,
            tr("Save file?"),
            tr("Would you like to save oppened file?")
        );
        if (reply == QMessageBox::Yes) {
            onSaveActionTriggered();
        }
    }
}

void MainWindow::onNewActionTriggered() {
    auto* geom = scene->getGeometry();

    askForSave();

    geom->clear();
    geom->setChanged(false);
    updateTitle();
}

void MainWindow::onSaveActionTriggered() {
    if (openedFileName.isEmpty()) {
        onSaveAsActionTriggered();
        return;
    }

    auto* geom = scene->getGeometry();

    try {
        geom->save(openedFileName);
        updateTitle();
        geom->setChanged(false);
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

    auto* geom = scene->getGeometry();

    try {
        geom->save(fileName);
        openedFileName = fileName;
        updateTitle();
        geom->setChanged(false);
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
        geom->setChanged(false);
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

void MainWindow::closeEvent(QCloseEvent* e) {
    askForSave();
    e->accept();
}
