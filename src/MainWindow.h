#pragma once

#include <QMainWindow>

class Function;
class QGraphicsScene;
class QGraphicsView;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createUi();

        void createFunctionActions();

        Function* currentFunction;

        QGraphicsScene* scene;
        QGraphicsView* view;

    private slots:
        void functionActionTriggered();
};
