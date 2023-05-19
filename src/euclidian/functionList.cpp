#include "Geometry.h"

#include "Line.h"
#include "Point.h"
#include "Circle.h"

#include "core/SectionMaster.h"
#include "core/Section.h"
#include "core/Function.h"
#include "core/Real.h"
#include "core/ArgumentInfo.h"

#include "util/TR.h"
#include "util/math.h"
#include "util/cramer.h"

#include <QDebug>
#include <QIcon>

#define DO [=](const QList<const Object*>& objs) -> QList<Object*>
#define ARGS QList<ArgumentInfo>

SectionMaster* Geometry::makeSectionMaster() const {
    auto* master = new SectionMaster;

    // -------------------- Sections --------------------
    auto* specialSection = master->makeSection(TR("Special"));
    auto* pointSection = master->makeSection(TR("Point"));
    auto* lineSection = master->makeSection(TR("Line"));
    auto* circleSection = master->makeSection(TR("Circle"));

    // -------------------- Special Section --------------------

    specialSection->makeEditMode(
        EditMode::Type::MOVE,
        TR("Move"),
        QIcon(QIcon(":none.svg")),
        TR(
            "Drag any free object with mouse to move it. "
            "Use middle mouse button to move around scene."
        )
    );

    specialSection->makeEditMode(
        EditMode::Type::HIDE,
        TR("Hide/Show"),
        QIcon(QIcon(":none.svg")),
        TR("Click on any object to toggle it's visibility.")
    );

    specialSection->makeEditMode(
        EditMode::Type::REMOVE,
        TR("Remove"),
        QIcon(QIcon(":none.svg")),
        TR("Click on any object to delete it.")
    );

    // -------------------- Point Section --------------------

    pointSection->makeEditMode(
        EditMode::Type::CREATE_POINT,
        TR("Create"),
        QIcon(QIcon(":none.svg")),
        TR("Click on the canvas to create a point.")
    );

    pointSection->makeFunction(
        "Intersect",
        QIcon(":none.svg"),
        TR("Intersects two objects."),
        ARGS {
            {Line::Type | Circle::Type, TR("First object")},
            {Line::Type | Circle::Type, TR("Second object")},
        },
        2,
        DO {
            if (objs[0]->is(Line::Type) && objs[1]->is(Line::Type)) {
                const auto& l1 = *static_cast<const Line*>(objs[0]);
                const auto& l2 = *static_cast<const Line*>(objs[1]);

                auto [a1, b1, c1] = l1.getABC();
                auto [a2, b2, c2] = l2.getABC();

                auto cramerAns = cramer({
                    {a1, b1, -c1},
                    {a2, b2, -c2},
                });

                if (cramerAns.isEmpty())
                    return {};

                return {new Point(cramerAns[0], cramerAns[1])};
            } else if (objs[0]->is(Circle::Type) && objs[1]->is(Circle::Type)) {
                const auto& w1 = *static_cast<const Circle*>(objs[0]);
                const auto& w2 = *static_cast<const Circle*>(objs[1]);

                //    /\
                // r1/   \ r2
                //  /a     \
                //  ---------
                //      d

                auto r1 = w1.getR();
                auto r2 = w2.getR();
                auto o1 = w1.getO();
                auto o2 = w2.getO();
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

                auto v = norm(o2 - o1) * r1;

                return {
                    new Point(rot(v, +sin_a, cos_a) + o1),
                    new Point(rot(v, -sin_a, cos_a) + o1),
                };

            } else {
                const Line* lPtr;
                const Circle* wPtr;
                if (objs[0]->is(Line::Type)) {
                    lPtr = static_cast<const Line*>(objs[0]);
                    wPtr = static_cast<const Circle*>(objs[1]);
                } else {
                    lPtr = static_cast<const Line*>(objs[1]);
                    wPtr = static_cast<const Circle*>(objs[0]);
                }

                const auto& l = *lPtr;
                const auto& w = *wPtr;

                auto o = w.getO();
                auto r = w.getR();

                double d = dist(o, l);
                if (gr(d, r))
                    return {};

                auto h = o + norm(l) * d;
                if (!eq(dist(h, l), 0)) {
                    h = o - norm(l) * d;
                }
                if (eq(d, r))
                    return {new Point(h)};

                auto x = sqrt(sq(r) - sq(d)) * dir(l);

                return {
                    new Point(h + x),
                    new Point(h - x),
                };
            }
        }
    );
    
    pointSection->makeFunction(
        "Middle",
        QIcon(":none.svg"),
        TR("Creates point between two current."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Second point")},
        },
        1,
        DO {
            const auto& p1 = *static_cast<const Point*>(objs[0]);
            const auto& p2 = *static_cast<const Point*>(objs[1]);

            return {new Point((p1 + p2) / 2)};
        }
    );

    // -------------------- Line Section --------------------

    lineSection->makeFunction(
        "By Two Points",
        QIcon(":none.svg"),
        TR("Creates line by two points."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Second point")},
        },
        1,
        DO {
            const auto& p1 = *static_cast<const Point*>(objs[0]);
            const auto& p2 = *static_cast<const Point*>(objs[1]);

            if (p1 == p2)
                return {};

            return {new Line(p1, p2)};
        }
    );

    lineSection->makeFunction(
        "Perpendicular",
        QIcon(":none.svg"),
        TR("Creates line perpendicular to current line through current point."),
        ARGS {
            {Point::Type, TR("Point")},
            {Line::Type, TR("Line")},
        },
        1,
        DO {
            const auto& a = *static_cast<const Point*>(objs[0]);
            const auto& l = *static_cast<const Line*>(objs[1]);

            return { new Line(a, a + norm(l)) };
        }
    );

    lineSection->makeFunction(
        "Parallel",
        QIcon(":none.svg"),
        TR("Creates line parallel to current line through current point."),
        ARGS {
            {Point::Type, TR("Point")},
            {Line::Type, TR("Line")},
        },
        1,
        DO {
            const auto& a = *static_cast<const Point*>(objs[0]);
            const auto& l = *static_cast<const Line*>(objs[1]);

            return { new Line(a, a + dir(l)) };
        }
    );

    auto* tangents = lineSection->makeFunction(
        "Tangents",
        QIcon(":none.svg"),
        TR("Creates tangents to circle from point."),
        ARGS {
            {Point::Type, TR("Point")},
            {Circle::Type, TR("Circle")},
        },
        2,
        DO {
            const auto& p = *static_cast<const Point*>(objs[0]);
            const auto& w = *static_cast<const Circle*>(objs[1]);

            auto o = w.getO();
            auto r = w.getR();
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
    );

    lineSection->makeFunction(
        "Common Tangent",
        QIcon(":none.svg"),
        TR("Creates common tangents for two circle."),
        ARGS {
            {Circle::Type, TR("Fistt circle")},
            {Circle::Type, TR("Second circle")},
        },
        2,
        DO {
            auto w1 = *static_cast<const Circle*>(objs[0]);
            auto w2 = *static_cast<const Circle*>(objs[1]);

            if (gr(w1.getR(), w2.getR())) {
                std::swap(w1, w2);
            }

            // r2 > r1
            auto o1 = w1.getO();
            auto o2 = w2.getO();
            auto r1 = w1.getR();
            auto r2 = w2.getR();

            if (gr(r2 - r1, dist(o1, o2))) {
                return {};
            }

            if (eq(r2 - r1, dist(o1, o2))) {
                auto v = norm(o1 - o2) * r2;
                auto p = o2 + v;
                return { new Line(p, p + perp(v)) };
            }

            if (eq(r1, r2)) {
                Line o1o2(o1, o2);
                auto d = norm(o1o2) * r1;
                return {
                    new Line(o1 + d, o2 + d),
                    new Line(o1 - d, o2 - d)
                };
            }

            Circle w(o2, r2 - r1);
            auto tangs = (*tangents)({&o1, &w});

            QList<Object*> ans;
            for (int i = 0; i < 2; ++i) {
                const auto* lPtr = static_cast<const Line*>(tangs[i]);
                const auto& l = *lPtr;
                auto [p1, p2] = l.getTwoPoints();

                auto d = norm(l) * r1;
                auto L = Line(p1 + d, p2 + d);
                if (!eq(r2, dist(L, o2))) {
                    L = Line(p1 - d, p2 - d);
                }

                ans << new Line(L);
                delete lPtr;
            }
            return ans;
        }
    );

    lineSection->makeFunction(
        "Bisector",
        QIcon(":none.svg"),
        TR("Creates bisector line of angle formed by three points."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Angle vertex")},
            {Point::Type, TR("Second point")},
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
    );

    // -------------------- Circle Section --------------------

    circleSection->makeFunction(
        "By Center And Point",
        QIcon(":none.svg"),
        TR("Creates circle by center and point on it."),
        ARGS {
            {Point::Type, TR("Center")},
            {Point::Type, TR("Point on circle")},
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
    );

    circleSection->makeFunction(
        "By Center And Radius",
        QIcon(":none.svg"),
        TR("Creates circle by its center and radius."),
        ARGS {
            {Point::Type, TR("Center")},
            {Real::Type, TR("Radius")},
        },
        1,
        DO {
            const auto& o = *static_cast<const Point*>(objs[0]);
            auto r = static_cast<const Real*>(objs[1])->value;

            if (leq(r, 0))
                return {};

            return {new Circle(o, r)};
        }
    );

    circleSection->makeFunction(
        "By Three Points",
        QIcon(":none.svg"),
        TR("Creates circle by three points on it."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Second point")},
            {Point::Type, TR("Third point")},
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
    );

    circleSection->makeFunction(
        "Incircle",
        QIcon(":none.svg"),
        TR("Incircle by three vertexes of triangle."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Second point")},
            {Point::Type, TR("Third point")},
        },
        1,
        DO {
            const auto& A = *static_cast<const Point*>(objs[0]);
            const auto& B = *static_cast<const Point*>(objs[1]);
            const auto& C = *static_cast<const Point*>(objs[2]);

            // Calc center
            auto a = dist(B, C);
            auto b = dist(A, C);
            auto c = dist(A, B);

            auto O = (a * A + b * B + c * C) / (a + b + c);

            // Calc radius
            auto p = (a + b + c) / 2;
            auto S = sqrt(p * (p - a) * (p - b) * (p - c));
            auto r = S / p;

            return { new Circle(O, r) };
        }
    );

    return master;
}
