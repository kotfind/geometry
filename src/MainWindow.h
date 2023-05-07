#pragma once

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
class EditMode;
class ToolWidget;
class VariableModel;
class Geometry;
class VariableWidget;

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

        Geometry* geom;

        QString openedFileName = "";

        Scene* scene;
        View* view;

        ToolInfoWidget* toolInfoWidget;
        ToolWidget* toolWidget;

        QHash<const EditMode*, QAction*> modeToAction;
        QHash<const Function*, QAction*> funcToAction;

        VariableModel* varModel;
        VariableWidget* varWidget;

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
