#include "core/MainWindow.h"
#include "core/Engine.h"
#include "core/EditMode.h"

#include "euclidian/Geometry.h"
#include "hyperbolic/Geometry.h"

#include <QApplication>

class AbstractGeometry;

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("kotfindApps");
    QCoreApplication::setApplicationName("Geometry");

    EditMode::init();

    euclidian::Geometry eGeom;
    hyperbolic::Geometry hGeom;

    Engine engine({ &eGeom, &hGeom });

    MainWindow master(&engine);
    master.show();

    int code = app.exec();

    EditMode::cleanup();
}
