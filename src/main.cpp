#include <QApplication>

#include "MainWindow.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MainWindow master;
    master.show();

    return app.exec();
}
