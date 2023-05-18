#include <QApplication>

#include "MainWindow.h"
#include "Engine.h"
#include "EGeometry.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Engine engine(std::make_unique<EGeometry>());

    MainWindow master(&engine);
    master.show();

    return app.exec();
}
