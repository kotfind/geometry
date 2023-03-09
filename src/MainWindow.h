#pragma once

#include "EditMode.h"

#include <QMainWindow>

class QGraphicsView;
class Scene;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createUi();

        void createActionsMenu();

        Scene* scene;
        QGraphicsView* view;

    private slots:
        void onFunctionActionTriggered();
};
