#pragma once

#include <QMainWindow>

class Function;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void createUi();

        void createFunctionActions();

        Function* currentFunction;

    private slots:
        void functionActionTriggered();
};
