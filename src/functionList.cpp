#include "Function.h"
#include "Real.h"
#include "Line.h"
#include "Point.h"
#include "Circle.h"
#include "global.h"

#define CAT2(a,b) a##b // actually concatenate
#define CAT(a,b) CAT2(a,b) // force expand
#define FUNC Function CAT(function,__COUNTER__)

#define DO [](const QList<const Object*>& objs) -> QList<Object*>

FUNC {
    "lineByTwoPoints",
    {Point::type, Point::type},
    1,
    DO {
        const auto& p1 = *dynamic_cast<const Point*>(objs[0]);
        const auto& p2 = *dynamic_cast<const Point*>(objs[1]);

        if (p1 == p2)
            return {};

        Line l;

        if (eq(p1.x, p2.x)) {
            l.a = 1;
            l.b = 0;
            l.c = -p1.x;
            return {new Line(l)};
        }

        l.a = p2.y - p1.y;
        l.b = p1.x - p2.x;
        l.c = -(l.a * p1.x + l.b * p1.y);
        return {new Line(l)};
    }
};

FUNC {
    "circleByCenterAndPoint",
    {Point::type, Point::type},
    1,
    DO {
        const auto& o = *dynamic_cast<const Point*>(objs[0]);
        const auto& p = *dynamic_cast<const Point*>(objs[1]);

        if (o == p)
            return {};

        auto r = dist(o, p);

        return {new Circle(o, r)};
    }
};

FUNC {
    "intersectTwoLines",
    {Line::type, Line::type},
    1,
    DO {
        const auto& l1 = *dynamic_cast<const Line*>(objs[0]);
        const auto& l2 = *dynamic_cast<const Line*>(objs[1]);

        double d  = l1.a * l2.b - l2.a * l1.b;
        double dx = l2.c * l1.b - l1.c * l2.b;
        double dy = l2.a * l1.c - l1.a * l2.c;

        if (eq(d, 0)) {
            return {};
        }

        return {new Point(dx / d, dy / d)};
    }
};
