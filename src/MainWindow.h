#pragma once

#include <QMainWindow>
#include <QString>
#include <QHash>
#include <memory>
#include <QMessageBox>

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
class QActionGroup;

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

        QMessageBox::StandardButton askForSave(bool addCancelButton);

        QString getFileNameFilter() const;

        Geometry* geom;

        QString openedFileName = "";

        Scene* scene;
        View* view;

        ToolInfoWidget* toolInfoWidget;
        ToolWidget* toolWidget;

        QActionGroup* toolActionGroup;
        QHash<const EditMode*, QAction*> modeToAction;
        QHash<const Function*, QAction*> funcToAction;

        VariableModel* varModel;
        VariableWidget* varWidget;

        static constexpr auto fileExtension = ".geom";

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
