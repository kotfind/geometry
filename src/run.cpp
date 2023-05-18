#include "run.h"

#include "MainWindow.h"
#include "Engine.h"

#include <QApplication>

class Geometry;

int run(int argc, char** argv, std::unique_ptr<Geometry> geom) {
    QApplication app(argc, argv);

    Engine engine(std::move(geom));

    MainWindow master(&engine);
    master.show();

    return app.exec();
}
