#include "Function.h"
#include "Real.h"
#include "Line.h"
#include "Point.h"
#include "Circle.h"

Function f{
    "lineByTwoPoints",
    {Point::type, Point::type},
    [](const QList<Object*>& objs) {
        // XXX it's not working
        auto* a = dynamic_cast<Point*>(objs[0]);
        auto* b = dynamic_cast<Point*>(objs[1]);
        return QList<Object*>{new Line()};
    }
};
