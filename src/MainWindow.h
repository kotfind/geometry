#pragma once

#include "Geometry.h"

#include <QMainWindow>
#include <QString>
#include <QHash>
#include <memory>

class QGraphicsView;
class Scene;
class View;
class QString;
class QCloseEvent;
class ToolInfoWidget;
class Section;
class Function;
enum class EditMode;
class ToolWidget;
class VariableModel;
class QTableView;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createModeAndFuncActions();

        void createUi();

        void createFileMenu();

        void createToolsMenu();

        void createDocks();

        void createDock(QWidget* widget, const QString& name, Qt::DockWidgetArea area);

        void updateTitle();

        void askForSave();

        std::unique_ptr<Geometry> geom;

        QString openedFileName = "";

        Scene* scene;
        View* view;

        ToolInfoWidget* toolInfoWidget;
        ToolWidget* toolWidget;

        QHash<EditMode, QAction*> modeToAction;
        QHash<Function*, QAction*> funcToAction;

        VariableModel* varModel;
        QTableView* varView;

    protected:
        void closeEvent(QCloseEvent*) override;

    private slots:
        void onFunctionActionTriggered();
        void onModeActionTriggered();

        void onNewActionTriggered();
        void onSaveActionTriggered();
        void onSaveAsActionTriggered();
        void onOpenActionTriggered();
};
