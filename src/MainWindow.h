#pragma once

#include "EditMode.h"

#include <QMainWindow>

class QGraphicsView;
class Scene;
class View;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createUi();

        void createActionsMenu();

        Scene* scene;
        View* view;

    private slots:
        void onFunctionActionTriggered();
};
