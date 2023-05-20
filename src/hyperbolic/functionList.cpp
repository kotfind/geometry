#include "Geometry.h"

#include "Point.h"
#include "Line.h"

#include "core/SectionMaster.h"
#include "core/Section.h"
#include "core/EditMode.h"

#include "util/TR.h"

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

    return master;
}
