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
class Engine;
class VariableWidget;
class QActionGroup;
class AbstractGeometry;
class QGraphicsItem;

class MainWindow : public QMainWindow {
    public:
        MainWindow(Engine* engine);

    private:
        void createUi();

        void createFileMenu();

        void initToolsMenu();

        void createDocks();
        void createDock(QWidget* widget, const QString& name, Qt::DockWidgetArea area);

        void updateTitle();

        void setActiveGeometry(const AbstractGeometry* geom);
        void setActiveGeometry(const QString& geomName);
        void getModeAndFunctionActions(
            QHash<const EditMode*, QAction*>& modeToAction,
            QHash<const Function*, QAction*>& funcToAction
        );
        QList<QMenu*> getToolMenus(
            const QHash<const EditMode*, QAction*>& modeToAction,
            const QHash<const Function*, QAction*>& funcToAction
        );

        bool askForSave();

        QString getFileNameFilter() const;

        Engine* engine;

        QString openedFileName = "";

        Scene* scene;
        View* view;

        QAction* toolsMenuSeparators[2];
        QActionGroup* toolActionGroup;

        ToolInfoWidget* toolInfoWidget;
        ToolWidget* toolWidget;

        VariableModel* varModel;
        VariableWidget* varWidget;

        QGraphicsItem* geometryGraphicsItem = nullptr;

        static constexpr auto fileExtension = ".geom";

    protected:
        void closeEvent(QCloseEvent*) override;

    private slots:
        void onFunctionActionTriggered();
        void onModeActionTriggered();

        bool save();
        bool saveAs();
        bool open();
        bool new_(const AbstractGeometry* geom);
};
