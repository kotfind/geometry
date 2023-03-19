#include "Function.h"
#include "Real.h"
#include "Line.h"
#include "Point.h"
#include "Circle.h"
#include "global.h"
#include <QDebug>

#define CAT2(a,b) a##b // actually concatenate
#define CAT(a,b) CAT2(a,b) // force expand
#define FUNC Function CAT(function,__COUNTER__)

#define DO [](const QList<const Object*>& objs) -> QList<Object*>

FUNC {
    "lineByTwoPoints",
    {Type::Point, Type::Point},
    1,
    DO {
        const auto& p1 = *static_cast<const Point*>(objs[0]);
        const auto& p2 = *static_cast<const Point*>(objs[1]);

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
    {Type::Point, Type::Point},
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
    {Type::Line | Type::Circle, Type::Line | Type::Circle},
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

            double r1 = w1.r;
            double r2 = w2.r;
            double d = dist(w1.o, w2.o);

            // XXX one intersection point

            if (!(le(abs(r1 - r2), d) && le(d, r1 + r2)))
                return {};

            double cos_a = (r1*r1 + d*d - r2*r2) /
                        // ---------------------
                               (2 * r1 * d);

            double sin_a = sqrt(1 - cos_a*cos_a);

            double x = (w2.o.x - w1.o.x) / d * r1;
            double y = (w2.o.y - w1.o.y) / d * r1;

            auto* p1 = new Point(
                x*cos_a - y*sin_a + w1.o.x,
                y*cos_a + x*sin_a + w1.o.y
            );

            auto* p2 = new Point(
                x*cos_a + y*sin_a + w1.o.x,
                y*cos_a - x*sin_a + w1.o.y
            );

            return {p1, p2};
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
