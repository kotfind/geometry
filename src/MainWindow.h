#pragma once

#include "EditMode.h"

#include <QMainWindow>
#include <QString>

class QGraphicsView;
class Scene;
class View;
class QString;
class QCloseEvent;
class ToolInfoWidget;
class Section;
class QTreeView;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createUi();

        void createFileMenu();

        void createToolsMenu();

        QMenu* getSectionMenu(Section*);

        void createDocks();

        void createDock(QWidget* widget, const QString& name);

        void updateTitle();

        void askForSave();

        QString openedFileName = "";

        Scene* scene;
        View* view;

        ToolInfoWidget* toolInfoWidget;

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
