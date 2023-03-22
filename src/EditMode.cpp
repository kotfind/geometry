#include "EditMode.h"

#include "global.h"

#include <QString>

QString modeName(EditMode m) {
    switch (m) {
        case EditMode::MOVE:
            return TR("Move");

        case EditMode::CREATE_POINT:
            return TR("Create Point");

        case EditMode::REMOVE:
            return TR("Remove");
    }
}

QString modeDescription(EditMode m) {
    switch (m) {
        case EditMode::MOVE:
            return TR(
                "Drag any free object with mouse to move it. "
                "Use middle mouse button to move around scene."
            );
        case EditMode::CREATE_POINT:
            return TR("Click on the canvas to create a point.");
        case EditMode::REMOVE:
            return TR("Click on any object to delete it.");
    }
}
