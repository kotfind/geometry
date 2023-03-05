#include "Function.h"
#include "Real.h"
#include "Line.h"
#include "Point.h"
#include "Circle.h"

#define CAT2(a,b) a##b // actually concatenate
#define CAT(a,b) CAT2(a,b) // force expand
#define FUNC Function CAT(function,__COUNTER__)

#define DO [](const QList<Object*>& objs) -> QList<Object*>

FUNC {
    "lineByTwoPoints",
    {Point::type, Point::type},
    DO {
        // XXX it's not working
        auto* a = dynamic_cast<Point*>(objs[0]);
        auto* b = dynamic_cast<Point*>(objs[1]);
        return {new Line()};
    }
};

FUNC {
    "lineByTwoPoints2",
    {Point::type, Point::type},
    DO {
        // XXX it's not working
        auto* a = dynamic_cast<Point*>(objs[0]);
        auto* b = dynamic_cast<Point*>(objs[1]);
        return QList<Object*>{new Line()};
    }
};
