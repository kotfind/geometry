#include "Function.h"
#include "Real.h"
#include "Line.h"
#include "Point.h"
#include "Circle.h"
#include "global.h"
#include "cramer.h"

#include <QDebug>

#define CAT2(a,b) a##b // actually concatenate
#define CAT(a,b) CAT2(a,b) // force expand
#define FUNC Function CAT(function,__COUNTER__)

#define DO [](const QList<const Object*>& objs) -> QList<Object*>

// -------------------- General Section --------------------

FUNC {
    "general",
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

            auto cramerAns = cramer({
                {l1.a, l1.b, -l1.c},
                {l2.a, l2.b, -l2.c},
            });

            if (cramerAns.isEmpty())
                return {};

            return {new Point(cramerAns[0], cramerAns[1])};
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
    "general",
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

// -------------------- Points Section --------------------

FUNC {
    "points",
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

// -------------------- Line Section --------------------

FUNC {
    "line",
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
    "line",
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
    "line",
    "tangents",
    TR("Creates tangents to circle from point."),
    {
        {Type::Point, TR("Point")},
        {Type::Circle, TR("Circle")},
    },
    2,
    DO {
        const auto& p = *static_cast<const Point*>(objs[0]);
        const auto& w = *static_cast<const Circle*>(objs[1]);

        const auto& o = w.o;
        const auto& r = w.r;
        auto d = dist(p, o);

        if (le(d, r))
            return {};

        if (eq(d, r)) {
            return { new Line(p, p + perp(o - p)) };
        }

        auto x = sqrt(sq(d) - sq(r));
        auto sin_a = r / d;
        auto cos_a = x / d;

        auto v = norm(o - p) * x;

        return {
            new Line(p, p + rot(v, +sin_a, cos_a)),
            new Line(p, p + rot(v, -sin_a, cos_a))
        };
    }
};

// -------------------- Circle Section --------------------

FUNC {
    "circle",
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
    "circle",
    "circleByThreePoints",
    TR("Creates circle by three points on it."),
    {
        {Type::Point, TR("First point")},
        {Type::Point, TR("Second point")},
        {Type::Point, TR("Third point")},
    },
    1,
    DO {
        const auto& [x1, y1] = *static_cast<const Point*>(objs[0]);
        const auto& [x2, y2] = *static_cast<const Point*>(objs[1]);
        const auto& [x3, y3] = *static_cast<const Point*>(objs[2]);

        // (x - x_0)^2 + (y - y_0)^2 = r^2
        // M = r^2 - x_0^2 - y_0^2

        auto cramerAns = cramer({
        //    x0    y0   M
        //  ---------------
            {2*x1, 2*y1, 1, sq(x1) + sq(y1)},
            {2*x2, 2*y2, 1, sq(x2) + sq(y2)},
            {2*x3, 2*y3, 1, sq(x3) + sq(y3)},
        });

        if (cramerAns.isEmpty())
            return {};

        auto x0 = cramerAns[0];
        auto y0 = cramerAns[1];
        auto M  = cramerAns[2];

        auto r = sqrt(M + sq(x0) + sq(y0));

        return { new Circle(Point(x0, y0), r) };
    }
};
