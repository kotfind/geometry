#pragma once

#include "EditMode.h"

#include <QMainWindow>
#include <QString>

class QGraphicsView;
class Scene;
class View;
class QString;
class QCloseEvent;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createUi();

        void createFileMenu();

        void createActionsMenu();

        QAction* createModeAction(const QString& name, EditMode mode);

        void updateTitle();

        void askForSave();

        QAction* newAction;
        QAction* saveAction;
        QAction* saveAsAction;
        QAction* openAction;

        Scene* scene;
        View* view;

        QString openedFileName = "";

    protected:
        void closeEvent(QCloseEvent*) override;

    private slots:
        void onFunctionActionTriggered();

        void onNewActionTriggered();
        void onSaveActionTriggered();
        void onSaveAsActionTriggered();
        void onOpenActionTriggered();
};
