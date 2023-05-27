#include "run.h"

#include "MainWindow.h"
#include "Engine.h"

#include <QApplication>

class AbstractGeometry;

int run(int argc, char** argv, const QList<AbstractGeometry*>& geoms) {
    QApplication app(argc, argv);

    Engine engine(geoms);

    MainWindow master(&engine);
    master.show();

    return app.exec();
}
