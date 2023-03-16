#pragma once

#include "EditMode.h"

#include <QMainWindow>
#include <QString>

class QGraphicsView;
class Scene;
class View;
class QString;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createUi();

        void createFileMenu();

        void createActionsMenu();

        QAction* createModeAction(const QString& name, EditMode mode);

        QAction* newAction;
        QAction* saveAction;
        QAction* saveAsAction;

        Scene* scene;
        View* view;

        QString openedFileName = "";

    private slots:
        void onFunctionActionTriggered();

        void onNewActionTriggered();
        void onSaveActionTriggered();
        void onSaveAsActionTriggered();
};
