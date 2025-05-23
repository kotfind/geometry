#include "Geometry.h"

#include "Point.h"
#include "Line.h"
#include "Circle.h"

#include "core/SectionMaster.h"
#include "core/Section.h"
#include "core/EditMode.h"
#include "core/Real.h"

#include "util/TR.h"
#include "util/cramer.h"

#include <QIcon>

#define DO [=](const QList<const Object*>& objs) -> QList<Object*>
#define ARGS QList<ArgumentInfo>

using namespace hyperbolic;
using namespace hyperbolic::impl;

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
            {Line::Type | Circle::Type, TR("First object")},
            {Line::Type | Circle::Type, TR("Second object")},
        },
        2,
        false,
        DO {
            if (objs[0]->is(Line::Type) && objs[1]->is(Line::Type)) {
                const auto& l1 = *static_cast<const Line*>(objs[0]);
                const auto& l2 = *static_cast<const Line*>(objs[1]);

                auto ans = intersect(l1, l2);

                if (ans.has_value()) {
                    return { ans->clone() };
                } else {
                    return {};
                }
            } else if (objs[0]->is(Circle::Type) && objs[1]->is(Circle::Type)) {
                const auto& w1 = *static_cast<const Circle*>(objs[0]);
                const auto& w2 = *static_cast<const Circle*>(objs[1]);

                int n;
                Point p1, p2;
                intersect(w1, w2, n, p1, p2);

                switch (n) {
                    case 2:
                        return { new Point(p1), new Point(p2) };

                    case 1:
                        return { new Point(p1) };

                    default:
                        return {};
                }
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

                int n;
                Point p1, p2;
                intersect(w, l, n, p1, p2);

                switch (n) {
                    case 2:
                        return { new Point(p1), new Point(p2) };

                    case 1:
                        return { new Point(p1) };

                    default:
                        return {};
                }
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

            return { new Point(midpoint(p1, p2)) };
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

            return {new Line(p1, p2)};
        }
    );

    lineSection->makeFunction(
        "Perpendicular",
        QIcon(":line/Perpendicular.svg"),
        TR("Creates line perpendicular to current line through current point."),
        ARGS {
            {Point::Type, TR("Point")},
            {Line::Type, TR("Line")},
        },
        1,
        false,
        DO {
            const auto& p = *static_cast<const Point*>(objs[0]);
            const auto& l = *static_cast<const Line*>(objs[1]);

            return { perp(l, p).clone() };
        }
    );

    lineSection->makeFunction(
        "Horoparallel",
        QIcon(":line/Horoparallel.svg"),
        TR("Creates lines hyperparallel to current line through current point."),
        ARGS {
            {Point::Type, TR("Point")},
            {Line::Type, TR("Line")},
        },
        2,
        false,
        DO {
            const auto& p = *static_cast<const Point*>(objs[0]);
            const auto& l = *static_cast<const Line*>(objs[1]);

            auto x = p.x;
            auto y = p.y;

            auto [p1, p2] = getIntersectionsWithAbsolute(l);

            auto a1 = (p1 + p) / 2;
            auto a2 = (p2 + p) / 2;

            return {
                new Line(p, a1),
                new Line(p, a2),
            };
        }
    );

    lineSection->makeFunction(
        "Hyperparallel",
        QIcon(":line/Hyperparallel.svg"),
        TR("Creates lines hyperparallel to current line through current point."),
        ARGS {
            {Point::Type, TR("Point")},
            {Line::Type, TR("Line")},
        },
        1,
        false,
        DO {
            const auto& p = *static_cast<const Point*>(objs[0]);
            const auto& l = *static_cast<const Line*>(objs[1]);

            // Two lines with equations
            //     Ax + By + C = 0
            // and
            //     Dx + Ey + F = 0
            // are hyperparallel when
            //     A * E = D * B

            auto x = p.x;
            auto y = p.y;
            auto [a, b, c] = l.getABC();

            auto d = -a;
            auto e = -b;
            auto f = a * x + b * y;

            auto [p1, p2] = getTwoPointsOnLine(d, e, f);

            return { new Line(p1, p2) };
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

            return { new Circle(o, r) };
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

            return { new Circle(o, r) };
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

    measurementSection->makeFunction(
        "Radius",
        QIcon(":measurement/Radius.svg"),
        TR("Calculates radius or circle."),
        ARGS {
            {Circle::Type, TR("Circle")},
        },
        1,
        true,
        DO {
            const auto& w = *static_cast<const Circle*>(objs[0]);
            return { new Real(w.r) };
        }
    );

    return master;
}
