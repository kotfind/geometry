#include "Function.h"
#include "Real.h"
#include "Line.h"
#include "Point.h"
#include "Circle.h"
#include "global.h"

#define CAT2(a,b) a##b // actually concatenate
#define CAT(a,b) CAT2(a,b) // force expand
#define FUNC Function CAT(function,__COUNTER__)

#define DO [](const QList<Object*>& objs) -> QList<Object*>

FUNC {
    "lineByTwoPoints",
    {Point::type, Point::type},
    DO {
        auto* p1 = dynamic_cast<Point*>(objs[0]);
        auto* p2 = dynamic_cast<Point*>(objs[1]);

        if (*p1 == *p2)
            return {};

        auto* l = new Line;

        if (eq(p1->x, p2->x)) {
            l->a = 1;
            l->b = 0;
            l->c = -p1->x;
            return {l};
        }

        l->a = p2->y - p1->y;
        l->b = p1->x - p2->x;
        l->c = -(l->a * p1->x + l->b * p1->y);
        return {l};
    }
};

FUNC {
    "circleByCenterAndPoint",
    {Point::type, Point::type},
    DO {
        auto* o = dynamic_cast<Point*>(objs[0]);
        auto* p = dynamic_cast<Point*>(objs[1]);

        if (*o == *p)
            return {};

        auto r = dist(*o, *p);

        return {new Circle(*o, r)};
    }
};
