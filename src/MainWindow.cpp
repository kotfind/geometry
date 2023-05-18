#include "MainWindow.h"

#include "Function.h"
#include "Scene.h"
#include "EditMode.h"
#include "View.h"
#include "Engine.h"
#include "IOError.h"
#include "ToolInfoWidget.h"
#include "Section.h"
#include "ToolWidget.h"
#include "VariableModel.h"
#include "RealGenerator.h"
#include "Real.h"
#include "VariableWidget.h"
#include "SectionMaster.h"

#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <QCloseEvent>
#include <QDockWidget>
#include <QTreeView>
#include <QTableView>
#include <QActionGroup>

MainWindow::MainWindow()
{
    engine = new Engine(this);

    updateTitle();

    createModeAndFuncActions();

    createUi();
    createFileMenu();
    createToolsMenu();
    createDocks();

    scene = new Scene(engine, this);
    view->setScene(scene);

    varModel = new VariableModel(engine, this);
    varWidget->setModel(varModel);

    connect(
        scene,
        &Scene::cursorChanged,
        view,
        &View::setCursor
    );

    connect(
        engine,
        &Engine::selectedCountChanged,
        toolInfoWidget,
        &ToolInfoWidget::updateSelectedCount
    );

    modeToAction[EditMode::get(EditMode::Type::MOVE)]->setChecked(true);
    engine->setEditMode(EditMode::get(EditMode::Type::MOVE));
    toolInfoWidget->setMode(EditMode::get(EditMode::Type::MOVE));
}

void MainWindow::createModeAndFuncActions() {
    toolActionGroup = new QActionGroup(this);
    toolActionGroup->setExclusive(true);
    toolActionGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);

    for (auto* section : engine->getGeometry()->getSectionMaster()->getSections()) {
        for (auto mode : section->getModes()) {
            auto* action = new QAction(
                mode->getIcon(),
                mode->getName(),
                this
            );
            action->setData(QVariant::fromValue(mode));
            toolActionGroup->addAction(action);
            action->setCheckable(true);
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
            toolActionGroup->addAction(action);
            action->setCheckable(true);
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

    for (auto* section : engine->getGeometry()->getSectionMaster()->getSections()) {
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
    toolInfoWidget = new ToolInfoWidget(engine->getGeometry(), this);
    createDock(toolInfoWidget, tr("Tool Info"), Qt::LeftDockWidgetArea);

    toolWidget = new ToolWidget(
        modeToAction,
        funcToAction,
        engine->getGeometry()->getSectionMaster(),
        this
    );
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
    auto* func = action->data().value<const Function*>();

    if (engine->getEditMode()->getType() == EditMode::Type::HIDE) {
        scene->update();
    }

    engine->setEditMode(EditMode::get(EditMode::Type::FUNCTION));
    engine->setActiveFunction(func, scene);
    toolInfoWidget->setMode(EditMode::get(EditMode::Type::FUNCTION)); // XXX: use Engine mode ?
    toolInfoWidget->setFunction(func); // XXX: use Engine function ?
}

void MainWindow::onModeActionTriggered() {
    auto* action = static_cast<QAction*>(sender());
    auto mode = action->data().value<const EditMode*>();

    if (engine->getEditMode()->getType() == EditMode::Type::HIDE ||
            mode->getType() == EditMode::Type::HIDE) {
        scene->update();
    }

    engine->setEditMode(mode);
    toolInfoWidget->setMode(mode);
}

QMessageBox::StandardButton MainWindow::askForSave(bool addCancelButton) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Save changes?"));
    msgBox.setText(tr(
        "You have unsaved changes."
        "Would you like to save them?"
    ));
    auto buttons = QMessageBox::Save | QMessageBox::Discard;
    if (addCancelButton) {
        buttons |= QMessageBox::Cancel;
    }
    msgBox.setStandardButtons(buttons);
    msgBox.setIcon(QMessageBox::Question);

    return static_cast<QMessageBox::StandardButton>(msgBox.exec());
}

QString MainWindow::getFileNameFilter() const {
    return tr("Geometry file (*%1)").arg(fileExtension);
}

void MainWindow::onNewActionTriggered() {
    if (engine->isChanged() && askForSave(false) == QMessageBox::Save) {
        onSaveActionTriggered();
    }

    engine->clear();
    openedFileName = "";
    updateTitle();
}

void MainWindow::onSaveActionTriggered() {
    if (openedFileName.isEmpty()) {
        onSaveAsActionTriggered();
        return;
    }

    try {
        engine->save(openedFileName);
        updateTitle();
        engine->setChanged(false);
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
        tr("Save file"),
        "",
        getFileNameFilter()
    );

    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(fileExtension)) {
        fileName += fileExtension;
    }

    try {
        engine->save(fileName);
        openedFileName = fileName;
        updateTitle();
        engine->setChanged(false);
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
        tr("Open file"),
        "",
        getFileNameFilter()
    );

    if (fileName.isEmpty()) return;

    try {
        engine->load(fileName);
        engine->populateScene(scene);
        openedFileName = fileName;

        updateTitle();
        engine->setChanged(false);
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
    if (!engine->isChanged()) {
        e->accept();
        return;
    }

    switch (askForSave(true)) {
        case QMessageBox::Save:
            onSaveActionTriggered();

        case QMessageBox::Discard:
            e->accept();
            break;

        case QMessageBox::Cancel:
            e->ignore();
            break;
    }
}
