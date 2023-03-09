#pragma once

#include "EditMode.h"

#include <QMainWindow>

class Function;
class QGraphicsScene;
class QGraphicsView;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createUi();

        void createActionsMenu();

        EditMode mode = EditMode::MOVE;
        Function* currentFunction = nullptr;

        QGraphicsScene* scene;
        QGraphicsView* view;

    private slots:
        void onFunctionActionTriggered();
};
