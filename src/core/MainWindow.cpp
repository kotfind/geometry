#include "MainWindow.h"

#include "Function.h"
#include "Scene.h"
#include "EditMode.h"
#include "View.h"
#include "Engine.h"
#include "ToolInfoWidget.h"
#include "Section.h"
#include "ToolWidget.h"
#include "VariableModel.h"
#include "RealGenerator.h"
#include "Real.h"
#include "VariableWidget.h"
#include "SectionMaster.h"

#include "util/IOError.h"

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
#include <QGraphicsItem>
#include <QSettings>

MainWindow::MainWindow(Engine* engine)
  : engine(engine)
{
    updateTitle();

    createUi();
    createFileMenu();
    initToolsMenu();
    createDocks();

    scene = new Scene(engine, this);
    view->setScene(scene);

    varModel = new VariableModel(engine, this);
    varWidget->setModel(varModel);

    toolActionGroup = new QActionGroup(this);
    toolActionGroup->setExclusive(true);
    toolActionGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);

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

    QSettings settings;
    if (settings.contains("activeGeometryName")) {
        setActiveGeometry(settings.value("activeGeometryName").toString());
    } else {
        setActiveGeometry(engine->getAllGeometries()[0]);
    }
    updateTitle();
}

void MainWindow::createUi() {
    view = new View(this);
    view->setMouseTracking(true);
    setCentralWidget(view);
}

void MainWindow::createFileMenu() {
    auto* menu = menuBar()->addMenu(tr("File"));

    for (const auto* geom : engine->getAllGeometries()) {
        auto* newAction = new QAction(
            tr("New %1").arg(geom->getName()),
            this
        );
        newAction->setData(QVariant::fromValue(geom));
        // newAction->setShortcut(Qt::CTRL | Qt::Key_N);
        connect(
            newAction,
            &QAction::triggered,
            [this, geom] () {
                new_(geom);
            }
        );
        menu->addAction(newAction);
    }

    auto* saveAction = new QAction(tr("Save"), this);
    saveAction->setShortcut(Qt::CTRL | Qt::Key_S);
    connect(
        saveAction,
        &QAction::triggered,
        this,
        &MainWindow::save
    );
    menu->addAction(saveAction);

    auto* saveAsAction = new QAction(tr("Save As"), this);
    saveAsAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S);
    connect(
        saveAsAction,
        &QAction::triggered,
        this,
        &MainWindow::saveAs
    );
    menu->addAction(saveAsAction);

    auto* openAction = new QAction(tr("Open"), this);
    openAction->setShortcut(Qt::CTRL | Qt::Key_O);
    connect(
        openAction,
        &QAction::triggered,
        this,
        &MainWindow::open
    );
    menu->addAction(openAction);
}

void MainWindow::initToolsMenu() {
    for (int i = 0; i < 2; ++i) {
        menuBar()->addAction(
            toolsMenuSeparators[i] = new QAction("|", this)
        );
    }
}

void MainWindow::createDocks() {
    toolInfoWidget = new ToolInfoWidget(engine->getActiveGeometry(), this);
    createDock(toolInfoWidget, tr("Tool Info"), Qt::LeftDockWidgetArea);

    toolWidget = new ToolWidget(this);
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

bool MainWindow::askForSave() {
    if (!engine->isChanged()) return true;

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Save changes?"));
    msgBox.setText(tr(
        "You have unsaved changes."
        "Would you like to save them?"
    ));
    msgBox.setStandardButtons(
        QMessageBox::Save |
        QMessageBox::Discard |
        QMessageBox::Cancel
    );
    msgBox.setIcon(QMessageBox::Question);

    switch (msgBox.exec()) {
        case QMessageBox::Save:
            return save();

        case QMessageBox::Discard:
            return true;

        case QMessageBox::Cancel:
            return false;
    }
}

QString MainWindow::getFileNameFilter() const {
    return tr("Geometry file (*%1)").arg(fileExtension);
}

bool MainWindow::save() {
    if (openedFileName.isEmpty()) {
        return saveAs();
    }

    try {
        engine->save(openedFileName);
        updateTitle();
        engine->setChanged(false);

        return true;
    } catch (const IOError& err) {
        QMessageBox::critical(
            this,
            tr("Input/ Output error"),
            err.what()
        );

        return false;
    }
}

bool MainWindow::saveAs() {
    auto fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save file"),
        "",
        getFileNameFilter()
    );

    if (fileName.isEmpty()) return false;

    if (!fileName.endsWith(fileExtension)) {
        fileName += fileExtension;
    }

    try {
        engine->save(fileName);
        openedFileName = fileName;
        updateTitle();
        engine->setChanged(false);

        return true;
    } catch (const IOError& err) {
        QMessageBox::critical(
            this,
            tr("Input/ Output error"),
            err.what()
        );

        return false;
    }
}

bool MainWindow::open() {
    if (!askForSave()) return false;

    auto fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        "",
        getFileNameFilter()
    );

    if (fileName.isEmpty()) return false;

    scene->detachAll();
    engine->clear();

    // FIXME: don't close file if load failed
    try {
        engine->load(fileName);
        setActiveGeometry(engine->getActiveGeometry()); // Geometry may have changed on load
        engine->populateScene(scene);
        openedFileName = fileName;
        engine->setChanged(false);
        updateTitle();

        return true;
    } catch (const IOError& err) {
        openedFileName = "";
        QMessageBox::critical(
            this,
            tr("Input/ Output error"),
            err.what()
        );

        return false;
    }
}

bool MainWindow::new_(const AbstractGeometry* geom) {
    if (!askForSave()) return false;

    scene->detachAll();
    engine->clear();
    setActiveGeometry(geom);
    engine->setChanged(false);
    openedFileName = "";
    updateTitle();

    return true;
}

void MainWindow::updateTitle() {
    setWindowTitle(openedFileName.isEmpty()
        ? tr("[Untitled]")
        : QFileInfo(openedFileName).fileName()
    );
}

void MainWindow::closeEvent(QCloseEvent* e) {
    if (!askForSave()) {
        e->ignore();
        return;
    }

    scene->detachAll();
    engine->clear();
    e->accept();
}

void MainWindow::setActiveGeometry(const AbstractGeometry* geom) {
    scene->detachAll();
    engine->setActiveGeometry(geom);
    geometryGraphicsItem = engine->getActiveGeometry()->getGraphicsItem();

    // Add geometry graphics item (if needed)
    if (auto* item = geometryGraphicsItem) {
        scene->addItem(item);
    }

    // Actions
    for (auto* action : toolActionGroup->actions()) {
        action->deleteLater();
    }

    QHash<const EditMode*, QAction*> modeToAction;
    QHash<const Function*, QAction*> funcToAction;
    getModeAndFunctionActions(modeToAction, funcToAction);

    // Menus
    QList<QAction*> menusToDelete;
    auto barActions = menuBar()->actions();
    for (
        int i = barActions.indexOf(toolsMenuSeparators[0]) + 1;
        i < barActions.indexOf(toolsMenuSeparators[1]);
        ++i
    ) {
        menuBar()->removeAction(barActions[i]);
    }

    auto menus = getToolMenus(modeToAction, funcToAction);
    for (auto* menu : menus) {
        menuBar()->insertMenu(
            toolsMenuSeparators[1],
            menu
        );
    }

    // ToolWidget
    toolWidget->update(
        modeToAction,
        funcToAction,
        engine->getActiveGeometry()->getSectionMaster()
    );

    // ToolInfoWidget
    toolInfoWidget->setActiveGeometry(geom);

    // Set move mode
    modeToAction[EditMode::get(EditMode::Type::MOVE)]->setChecked(true);
    engine->setEditMode(EditMode::get(EditMode::Type::MOVE));
    toolInfoWidget->setMode(EditMode::get(EditMode::Type::MOVE));

    // Save to settings
    QSettings settings;
    settings.setValue("activeGeometryName", geom->getName());
}

void MainWindow::setActiveGeometry(const QString& geomName) {
    for (auto* geom : engine->getAllGeometries()) {
        if (geom->getName() == geomName) {
            setActiveGeometry(geom);
            return;
        }
    }

    throw std::runtime_error("Geometry " + geomName.toStdString() + " was not loaded.");
}

void MainWindow::getModeAndFunctionActions(
    QHash<const EditMode*, QAction*>& modeToAction,
    QHash<const Function*, QAction*>& funcToAction
) {

    for (auto* section : engine->getActiveGeometry()->getSectionMaster()->getSections()) {
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

QList<QMenu*> MainWindow::getToolMenus(
    const QHash<const EditMode*, QAction*>& modeToAction,
    const QHash<const Function*, QAction*>& funcToAction
) {
    QList<QMenu*> menus;

    for (auto* section : engine->getActiveGeometry()->getSectionMaster()->getSections()) {
        auto* menu = menuBar()->addMenu(section->getName());

        for (auto mode : section->getModes()) {
            menu->addAction(modeToAction[mode]);
        }
        for (auto* func : section->getFunctions()) {
            menu->addAction(funcToAction[func]);
        }

        menus << menu;
    }

    return menus;
}

