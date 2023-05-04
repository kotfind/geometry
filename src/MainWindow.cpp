#include "MainWindow.h"

#include "Function.h"
#include "Scene.h"
#include "EditMode.h"
#include "View.h"
#include "Geometry.h"
#include "IOError.h"
#include "ToolInfoWidget.h"
#include "Section.h"
#include "ToolWidget.h"
#include "VariableModel.h"
#include "RealGenerator.h"
#include "Real.h"
#include "VariableWidget.h"

#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <QCloseEvent>
#include <QDockWidget>
#include <QTreeView>
#include <QTableView>

MainWindow::MainWindow()
{
    updateTitle();

    createModeAndFuncActions();

    createUi();
    createFileMenu();
    createToolsMenu();
    createDocks();

    geom = new Geometry(this);

    scene = new Scene(geom, this);
    view->setScene(scene);

    toolInfoWidget->setMode(EditMode::MOVE);

    varModel = new VariableModel(geom, this);
    varWidget->setModel(varModel);

    connect(
        scene,
        &Scene::cursorChanged,
        view,
        &View::setCursor
    );

    connect(
        geom,
        &Geometry::selectedCountChanged,
        toolInfoWidget,
        &ToolInfoWidget::updateSelectedCount
    );
}

void MainWindow::createModeAndFuncActions() {
    for (auto* section : Section::getMaster()->getSections()) {
        for (auto mode : section->getModes()) {
            auto* action = new QAction(
                modeIcon(mode),
                modeName(mode),
                this
            );
            action->setData(QVariant::fromValue(mode));
            connect(
                action,
                &QAction::triggered,
                this,
                &MainWindow::onModeActionTriggered
            );
            modeToAction[mode] = action;
        }
        for (auto* func : section->getFunctions()) {
            auto* action = new QAction(
                func->getIcon(),
                func->getSelfName(),
                this
            );
            action->setData(QVariant::fromValue(func));
            connect(
                action,
                &QAction::triggered,
                this,
                &MainWindow::onFunctionActionTriggered
            );
            funcToAction[func] = action;
        }
    }
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

    for (auto* section : Section::getMaster()->getSections()) {
        auto* menu = menuBar()->addMenu(section->getName());

        for (auto mode : section->getModes()) {
            menu->addAction(modeToAction[mode]);
        }
        for (auto* func : section->getFunctions()) {
            menu->addAction(funcToAction[func]);
        }
    }

    menuBar()->addAction(new QAction("|", this)); // Separator
}

void MainWindow::createDocks() {
    toolInfoWidget = new ToolInfoWidget(this);
    createDock(toolInfoWidget, tr("Tool Info"), Qt::LeftDockWidgetArea);

    toolWidget = new ToolWidget(modeToAction, funcToAction, this);
    createDock(toolWidget, tr("Tools"), Qt::LeftDockWidgetArea);

    varWidget = new VariableWidget(this);
    createDock(varWidget, tr("Variables"), Qt::RightDockWidgetArea);
}

void MainWindow::createDock(QWidget* widget, const QString& name, Qt::DockWidgetArea area) {
    auto* dock = new QDockWidget(name, this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(widget);
    addDockWidget(area, dock);

    static QMenu* viewMenu = menuBar()->addMenu(tr("View"));
    viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::onFunctionActionTriggered() {
    auto* action = static_cast<QAction*>(sender());
    auto* func = action->data().value<Function*>();

    geom->setEditMode(EditMode::FUNCTION);
    geom->setActiveFunction(func);
    toolInfoWidget->setMode(EditMode::FUNCTION); // XXX: use Geometry mode ?
    toolInfoWidget->setFunction(func); // XXX: use Geometry function ?
}

void MainWindow::onModeActionTriggered() {
    auto* action = static_cast<QAction*>(sender());
    auto mode = action->data().value<EditMode>();

    geom->setEditMode(mode);
    toolInfoWidget->setMode(mode);
}

void MainWindow::askForSave() {
    if (geom->isChanged()) {
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
