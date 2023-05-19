#include "run.h"

#include "MainWindow.h"
#include "Engine.h"

#include <QApplication>

class AbstractGeometry;

int run(int argc, char** argv, std::unique_ptr<AbstractGeometry> geom) {
    QApplication app(argc, argv);

    Engine engine(std::move(geom));

    MainWindow master(&engine);
    master.show();

    return app.exec();
}
