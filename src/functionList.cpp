#include "Function.h"
#include "Real.h"
#include "Line.h"
#include "Point.h"
#include "Circle.h"
#include "global.h"

#include <QDebug>
#include <QObject>

#define CAT2(a,b) a##b // actually concatenate
#define CAT(a,b) CAT2(a,b) // force expand
#define FUNC Function CAT(function,__COUNTER__)

#define DO [](const QList<const Object*>& objs) -> QList<Object*>
#define TR

FUNC {
    "line",
    TR("Creates line by two points."),
    {
        {Type::Point, TR("First point")},
        {Type::Point, TR("Second point")},
    },
    1,
    DO {
        const auto& p1 = *static_cast<const Point*>(objs[0]);
        const auto& p2 = *static_cast<const Point*>(objs[1]);

        if (p1 == p2)
            return {};

        return {new Line(p1, p2)};
    }
};

FUNC {
    "circleByCenterAndPoint",
    TR("Creates circle by center and point on it."),
    {
        {Type::Point, TR("Center")},
        {Type::Point, TR("Point on circle")},
    },
    1,
    DO {
        const auto& o = *static_cast<const Point*>(objs[0]);
        const auto& p = *static_cast<const Point*>(objs[1]);

        if (o == p)
            return {};

        auto r = dist(o, p);

        return {new Circle(o, r)};
    }
};

FUNC {
    "intersect",
    TR("Intersects two objects."),
    {
        {Type::Line | Type::Circle, TR("First object")},
        {Type::Line | Type::Circle, TR("Second object")},
    },
    2,
    DO {
        if (objs[0]->is(Type::Line) && objs[1]->is(Type::Line)) {
            const auto& l1 = *static_cast<const Line*>(objs[0]);
            const auto& l2 = *static_cast<const Line*>(objs[1]);

            double d  = l1.a * l2.b - l2.a * l1.b;
            double dx = l2.c * l1.b - l1.c * l2.b;
            double dy = l2.a * l1.c - l1.a * l2.c;

            if (eq(d, 0)) {
                return {};
            }

            return {new Point(dx / d, dy / d)};
        } else if (objs[0]->is(Type::Circle) && objs[1]->is(Type::Circle)) {
            const auto& w1 = *static_cast<const Circle*>(objs[0]);
            const auto& w2 = *static_cast<const Circle*>(objs[1]);

            //    /\
            // r1/   \ r2
            //  /a     \
            //  ---------
            //      d

            auto r1 = w1.r;
            auto r2 = w2.r;
            auto o1 = w1.o;
            auto o2 = w2.o;
            auto d = dist(o1, o2);

            if (gr(abs(r1 - r2), d) || gr(d, r1 + r2))
                return {};

            if (eq(abs(r1 - r2), d) || eq(d, r1 + r2)) {
                return { new Point(norm(o2 - o1) * r1 + o1) };
            }

            auto cos_a = (sq(r1) + sq(d) - sq(r2)) /
                      // ---------------------
                             (2 * r1 * d);

            auto sin_a = sqrt(1 - cos_a*cos_a);

            auto v = norm(w2.o - w1.o) * r1;

            return {
                new Point(rot(v, +sin_a, cos_a) + w1.o),
                new Point(rot(v, -sin_a, cos_a) + w1.o),
            };

        } else {
            const Line* lPtr;
            const Circle* wPtr;
            if (objs[0]->is(Type::Line)) {
                lPtr = static_cast<const Line*>(objs[0]);
                wPtr = static_cast<const Circle*>(objs[1]);
            } else {
                lPtr = static_cast<const Line*>(objs[1]);
                wPtr = static_cast<const Circle*>(objs[0]);
            }

            const auto& l = *lPtr;
            const auto& w = *wPtr;

            double d = dist(w.o, l);
            if (gr(d, w.r))
                return {};

            auto h = w.o + norm(l) * d;
            if (!eq(dist(h, l), 0)) {
                h = w.o - norm(l) * d;
            }
            if (eq(d, w.r))
                return {new Point(h)};

            auto x = sqrt(sq(w.r) - sq(d)) * dir(l);

            return {
                new Point(h + x),
                new Point(h - x),
            };
        }
    }
};

FUNC {
    "angleBisector",
    TR("Creates bisector line of angle formed by three points."),
    {
        {Type::Point, TR("First point")},
        {Type::Point, TR("Angle vertex")},
        {Type::Point, TR("Second point")},
    },
    1,
    DO {
        const auto& a = *static_cast<const Point*>(objs[0]);
        const auto& o = *static_cast<const Point*>(objs[1]);
        const auto& b = *static_cast<const Point*>(objs[2]);

        auto oa = a - o;
        auto ob = b - o;

        auto l1 = len(oa);
        auto l2 = len(ob);

        oa *= l2;
        ob *= l1;

        auto oc = oa + ob;

        return { new Line(o, o + oc) };
    }
};

FUNC {
    "perpendicular",
    TR("Creates line perpendicular to current line through current point."),
    {
        {Type::Point, TR("Point")},
        {Type::Line, TR("Line")},
    },
    1,
    DO {
        const auto& a = *static_cast<const Point*>(objs[0]);
        const auto& l = *static_cast<const Line*>(objs[1]);

        return { new Line(a, a + norm(l)) };
    }
};

FUNC {
    "midpoint",
    TR("Creates point between two current."),
    {
        {Type::Point, TR("First point")},
        {Type::Point, TR("Second point")},
    },
    1,
    DO {
        const auto& p1 = *static_cast<const Point*>(objs[0]);
        const auto& p2 = *static_cast<const Point*>(objs[1]);

        return {new Point((p1 + p2) / 2)};
    }
};
