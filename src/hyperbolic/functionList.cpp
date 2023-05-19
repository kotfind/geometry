#include "Geometry.h"

#include "Point.h"

#include "core/SectionMaster.h"
#include "core/Section.h"
#include "core/EditMode.h"

#include "util/TR.h"

using namespace hyperbolic;
using namespace hyperbolic::impl;

SectionMaster* Geometry::makeSectionMaster() const {
    auto* master = new SectionMaster;

    // -------------------- Sections --------------------

    auto* specialSection = master->makeSection(TR("Special"));
    auto* pointSection = master->makeSection(TR("Point"));

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


    return master;
}
