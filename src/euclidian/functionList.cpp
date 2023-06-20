#include "Geometry.h"

#include "Line.h"
#include "Point.h"
#include "Circle.h"
#include "Segment.h"

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

using namespace euclidian;
using namespace euclidian::impl;

SectionMaster* Geometry::makeSectionMaster() const {
    auto* master = new SectionMaster;

    // -------------------- Sections --------------------
    auto* specialSection = master->makeSection(TR("Special"));
    auto* pointSection = master->makeSection(TR("Point"));
    auto* lineSection = master->makeSection(TR("Line"));
    auto* circleSection = master->makeSection(TR("Circle"));
    auto* measurementSection = master->makeSection(TR("Measurement"));

    // -------------------- Special Section --------------------

    specialSection->addEditMode(EditMode::get(EditMode::Type::MOVE));
    specialSection->addEditMode(EditMode::get(EditMode::Type::HIDE));
    specialSection->addEditMode(EditMode::get(EditMode::Type::REMOVE));

    // -------------------- Point Section --------------------

    pointSection->addEditMode(EditMode::get(EditMode::Type::CREATE_POINT));

    pointSection->makeFunction(
        "Intersect",
        QIcon(":point/Intersect.svg"),
        TR("Intersects two objects."),
        ARGS {
            {Line::Type | Segment::Type | Circle::Type, TR("First object")},
            {Line::Type | Segment::Type | Circle::Type, TR("Second object")},
        },
        2,
        false,
        DO {
            if (objs[0]->is(Line::Type) && objs[1]->is(Line::Type)) {
                auto l1 = *static_cast<const Line*>(objs[0]);
                auto l2 = *static_cast<const Line*>(objs[1]);

                int n;
                Point p;
                intersect(l1, l2, n, p);

                if (!n) return {};
                return { new Point(p) };

            } else if (objs[0]->is(Line::Type) && objs[1]->is(Segment::Type)) {
                auto l = *static_cast<const Line*>(objs[0]);
                auto s = *static_cast<const Segment*>(objs[1]);

                int n;
                Point p;
                intersect(l, s.toLine(), n, p);

                if (!n) return {};
                if (!on(p, s)) return {};

                return { new Point(p) };

            } else if (objs[0]->is(Line::Type) && objs[1]->is(Circle::Type)) {
                auto l = *static_cast<const Line*>(objs[0]);
                auto w = *static_cast<const Circle*>(objs[1]);

                int n;
                Point p1, p2;

                intersect(l, w, n, p1, p2);

                QList<Object*> ans;
                if (n >= 1) ans << new Point(p1);
                if (n >= 2) ans << new Point(p2);
                return ans;
            } else if (objs[0]->is(Segment::Type) && objs[1]->is(Line::Type)) {
                auto s = *static_cast<const Segment*>(objs[0]);
                auto l = *static_cast<const Line*>(objs[1]);

                int n;
                Point p;
                intersect(l, s.toLine(), n, p);

                if (!n) return {};
                if (!on(p, s)) return {};

                return { new Point(p) };

            } else if (objs[0]->is(Segment::Type) && objs[1]->is(Segment::Type)) {
                auto s1 = *static_cast<const Segment*>(objs[0]);
                auto s2 = *static_cast<const Segment*>(objs[1]);

                int n;
                Point p;
                intersect(s1.toLine(), s2.toLine(), n, p);

                if (!n) return {};
                if (!on(p, s1)) return {};
                if (!on(p, s2)) return {};

                return { new Point(p) };

            } else if (objs[0]->is(Segment::Type) && objs[1]->is(Circle::Type)) {
                auto s = *static_cast<const Segment*>(objs[0]);
                auto w = *static_cast<const Circle*>(objs[1]);

                int n;
                Point p1, p2;

                intersect(s.toLine(), w, n, p1, p2);

                QList<Object*> ans;
                if (n >= 1 && on(p1, s)) ans << new Point(p1);
                if (n >= 2 && on(p2, s)) ans << new Point(p2);
                return ans;
            } else if (objs[0]->is(Circle::Type) && objs[1]->is(Line::Type)) {
                auto w = *static_cast<const Circle*>(objs[0]);
                auto l = *static_cast<const Line*>(objs[1]);

                int n;
                Point p1, p2;

                intersect(w, l, n, p1, p2);

                QList<Object*> ans;
                if (n >= 1) ans << new Point(p1);
                if (n >= 2) ans << new Point(p2);
                return ans;

            } else if (objs[0]->is(Circle::Type) && objs[1]->is(Segment::Type)) {
                auto w = *static_cast<const Circle*>(objs[0]);
                auto s = *static_cast<const Segment*>(objs[1]);

                int n;
                Point p1, p2;

                intersect(w, s.toLine(), n, p1, p2);

                QList<Object*> ans;
                if (n >= 1 && on(p1, s)) ans << new Point(p1);
                if (n >= 2 && on(p2, s)) ans << new Point(p2);
                return ans;

            } else if (objs[0]->is(Circle::Type) && objs[1]->is(Circle::Type)) {
                const auto& w1 = *static_cast<const Circle*>(objs[0]);
                const auto& w2 = *static_cast<const Circle*>(objs[1]);

                int n;
                Point p1, p2;

                intersect(w1, w2, n, p1, p2);

                QList<Object*> ans;
                if (n >= 1) ans << new Point(p1);
                if (n >= 2) ans << new Point(p2);
                return ans;
            } else {
                assert(false);
            }
        }
    );
    
    pointSection->makeFunction(
        "Middle",
        QIcon(":point/Middle.svg"),
        TR("Creates point between two current."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Second point")},
        },
        1,
        false,
        DO {
            const auto& p1 = *static_cast<const Point*>(objs[0]);
            const auto& p2 = *static_cast<const Point*>(objs[1]);

            return {new Point((p1 + p2) / 2)};
        }
    );

    // -------------------- Line Section --------------------

    lineSection->makeFunction(
        "By Two Points",
        QIcon(":line/ByTwoPoints.svg"),
        TR("Creates line by two points."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Second point")},
        },
        1,
        false,
        DO {
            const auto& p1 = *static_cast<const Point*>(objs[0]);
            const auto& p2 = *static_cast<const Point*>(objs[1]);

            if (p1 == p2)
                return {};

            return { new Line(p1, p2) };
        }
    );

    lineSection->makeFunction(
        "Segment",
        QIcon(":line/Segment.svg"),
        TR("Creates segment by two points."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Second point")},
        },
        1,
        false,
        DO {
            const auto& p1 = *static_cast<const Point*>(objs[0]);
            const auto& p2 = *static_cast<const Point*>(objs[1]);

            if (p1 == p2)
                return {};

            return { new Segment(p1, p2) };
        }
    );

    lineSection->makeFunction(
        "Perpendicular",
        QIcon(":line/Perpendicular.svg"),
        TR("Creates line perpendicular to current line through current point."),
        ARGS {
            {Point::Type, TR("Point")},
            {Line::Type | Segment::Type, TR("Line")},
        },
        1,
        false,
        DO {
            auto a = *static_cast<const Point*>(objs[0]);
            auto l = objs[1]->is(Segment::Type)
                ? static_cast<const Segment*>(objs[1])->toLine()
                : *static_cast<const Line*>(objs[1]);

            return { new Line(a, a + norm(l)) };
        }
    );

    lineSection->makeFunction(
        "Parallel",
        QIcon(":line/Parallel.svg"),
        TR("Creates line parallel to current line through current point."),
        ARGS {
            {Point::Type, TR("Point")},
            {Line::Type | Segment::Type, TR("Line")},
        },
        1,
        false,
        DO {
            auto a = *static_cast<const Point*>(objs[0]);
            auto l = objs[1]->is(Segment::Type)
                ? static_cast<const Segment*>(objs[1])->toLine()
                : *static_cast<const Line*>(objs[1]);

            return { new Line(a, a + dir(l)) };
        }
    );

    lineSection->makeFunction(
        "Bisector",
        QIcon(":line/Bisector.svg"),
        TR("Creates bisector line of angle formed by three points."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Angle vertex")},
            {Point::Type, TR("Second point")},
        },
        1,
        false,
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

    auto* tangents = lineSection->makeFunction(
        "Tangents",
        QIcon(":line/Tangents.svg"),
        TR("Creates tangents to circle from point."),
        ARGS {
            {Point::Type, TR("Point")},
            {Circle::Type, TR("Circle")},
        },
        2,
        false,
        DO {
            const auto& p = *static_cast<const Point*>(objs[0]);
            const auto& w = *static_cast<const Circle*>(objs[1]);

            auto o = w.o;
            auto r = w.r;
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
        "Common Tangents",
        QIcon(":line/CommonTangents.svg"),
        TR("Creates common tangents for two circle."),
        ARGS {
            {Circle::Type, TR("Fistt circle")},
            {Circle::Type, TR("Second circle")},
        },
        2,
        false,
        DO {
            auto w1 = *static_cast<const Circle*>(objs[0]);
            auto w2 = *static_cast<const Circle*>(objs[1]);

            if (gr(w1.r, w2.r)) {
                std::swap(w1, w2);
            }

            // r2 > r1
            auto o1 = w1.o;
            auto o2 = w2.o;
            auto r1 = w1.r;
            auto r2 = w2.r;

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
                auto p1 = l.p1;
                auto p2 = l.p2;

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

    // -------------------- Circle Section --------------------

    circleSection->makeFunction(
        "By Center And Point",
        QIcon(":circle/ByCenterAndPoint.svg"),
        TR("Creates circle by center and point on it."),
        ARGS {
            {Point::Type, TR("Center")},
            {Point::Type, TR("Point on circle")},
        },
        1,
        false,
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
        QIcon(":circle/ByCenterAndRadius.svg"),
        TR("Creates circle by its center and radius."),
        ARGS {
            {Point::Type, TR("Center")},
            {Real::Type, TR("Radius")},
        },
        1,
        false,
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
        QIcon(":circle/ByThreePoints.svg"),
        TR("Creates circle by three points on it."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Second point")},
            {Point::Type, TR("Third point")},
        },
        1,
        false,
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
        QIcon(":circle/Incircle.svg"),
        TR("Incircle by three vertexes of triangle."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Second point")},
            {Point::Type, TR("Third point")},
        },
        1,
        false,
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

    // -------------------- Measurement Section --------------------

    measurementSection->makeFunction(
        "Distance",
        QIcon(":measurement/Distance.svg"),
        TR("Calculates distance between two points."),
        ARGS {
            {Point::Type, TR("First point")},
            {Point::Type, TR("Second point")},
        },
        1,
        true,
        DO {
            const auto& a = *static_cast<const Point*>(objs[0]);
            const auto& b = *static_cast<const Point*>(objs[1]);

            return { new Real(dist(a, b)) };
        }
    );

    return master;
}
