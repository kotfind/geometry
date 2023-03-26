#pragma once

#include <QMainWindow>
#include <QString>
#include <QHash>

class QGraphicsView;
class Scene;
class View;
class QString;
class QCloseEvent;
class ToolInfoWidget;
class Section;
class Function;
enum class EditMode;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createModeAndFuncActions();

        void createUi();

        void createFileMenu();

        void createToolsMenu();

        void createDocks();

        void createDock(QWidget* widget, const QString& name);

        void updateTitle();

        void askForSave();

        QString openedFileName = "";

        Scene* scene;
        View* view;

        ToolInfoWidget* toolInfoWidget;

        QHash<Function*, QAction*> funcToAction;
        QHash<EditMode, QAction*> modeToAction;

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
