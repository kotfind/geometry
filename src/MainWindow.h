#pragma once

#include "EditMode.h"

#include <QMainWindow>

class QGraphicsView;
class Scene;
class View;
class QString;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createUi();

        void createActionsMenu();

        QAction* createModeAction(const QString& name, EditMode mode);

        Scene* scene;
        View* view;

    private slots:
        void onFunctionActionTriggered();
};
