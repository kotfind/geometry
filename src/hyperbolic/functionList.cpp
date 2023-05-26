#include "Geometry.h"

#include "Point.h"
#include "Line.h"

#include "core/SectionMaster.h"
#include "core/Section.h"
#include "core/EditMode.h"

#include "util/TR.h"
#include "util/cramer.h"

#include <QIcon>

#define DO [=](const QList<const Object*>& objs) -> QList<Object*>
#define ARGS QList<ArgumentInfo>

using namespace hyperbolic;
using namespace hyperbolic::impl;

std::pair<Point, Point> getTwoPointsOnLine(double a, double b, double c) {
    // Hs[0] and Hs[1] are intersections of line and unit circle
    // OM is perpendicular from O(0, 0) on line
    // Ans points are midpoins of M and Hs

    auto M = Point(a, b) * (-c / (sq(a) + sq(b)));

    Point Hs[2];

    if (eq(b, 0)) {
        auto x = Hs[0].x = Hs[1].x = -c / a;
        Hs[0].y = sqrt(1 - sq(x));
        Hs[1].y = -Hs[0].y;
    } else {
        int n;
        solveSqEq(
            sq(a) + sq(b),
            2 * a * c,
            sq(c) - sq(b),
            n,
            Hs[0].x,
            Hs[1].x
        );
        if (n != 2) {
            qDebug() << a << b << c;
        }
        assert(n == 2);
        for (int i = 0; i < 2; ++i) {
            Hs[i].y = -(a * Hs[i].x + c) / b;
        }
    }

    return {
        (Hs[0] + M) / 2,
        (Hs[1] + M) / 2
    };
}

SectionMaster* Geometry::makeSectionMaster() const {
    auto* master = new SectionMaster;

    // -------------------- Sections --------------------

    auto* specialSection = master->makeSection(TR("Special"));
    auto* pointSection = master->makeSection(TR("Point"));
    auto* lineSection = master->makeSection(TR("Line"));

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
        TR("Intersects two lines."),
        ARGS {
            {Line::Type, TR("First line")},
            {Line::Type, TR("Second line")},
        },
        1,
        DO {
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

            return { new Point(cramerAns[0], cramerAns[1]) };
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

            auto x1 = p1.x;
            auto y1 = p1.y;
            auto x2 = p2.x;
            auto y2 = p2.y;

            double t1 = sqrt(1 - sq(x1) - sq(y1));
            double t2 = sqrt(1 - sq(x2) - sq(y2));

            return { new Point(
                (x1 * t2 + x2 * t1) / (t1 + t2),
                (y1 * t2 + y2 * t1) / (t1 + t2)
            ) };
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
            const auto& p = *static_cast<const Point*>(objs[0]);
            const auto& l = *static_cast<const Line*>(objs[1]);

            // Two lines with equations
            //     Ax + By + C = 0
            // and
            //     Dx + Ey + F = 0
            // are perpendicular when
            //     AD + BE = CF

            auto x = p.x;
            auto y = p.y;
            auto [a, b, c] = l.getABC();

            auto d = c * y + b;
            auto e = -(c * x + a);
            auto f = -(d * x + e * y);

            auto [p1, p2] = getTwoPointsOnLine(d, e, f);

            return { new Line(p1, p2) };
        }
    );

    return master;
}
