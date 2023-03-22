#pragma once

#include "EditMode.h"

#include <QMainWindow>
#include <QString>

class QGraphicsView;
class Scene;
class View;
class QString;
class QCloseEvent;
class FunctionInfoWidget;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createUi();

        void createFileMenu();

        void createActionsMenu();

        void createDocks();

        void createDock(QWidget* widget, const QString& name);

        QAction* createModeAction(const QString& name, EditMode mode);

        void updateTitle();

        void askForSave();

        QString openedFileName = "";

        Scene* scene;
        View* view;

        FunctionInfoWidget* functionInfoWidget;

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
