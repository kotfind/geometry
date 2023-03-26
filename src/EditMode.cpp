#include "EditMode.h"

#include "global.h"
#include "Section.h"

#include <QString>
#include <QIcon>

QString modeName(EditMode m) {
    switch (m) {
        case EditMode::MOVE:
            return TR("Move");

        case EditMode::CREATE_POINT:
            return TR("Create");

        case EditMode::REMOVE:
            return TR("Remove");
    }
}

QIcon modeIcon(EditMode m) {
    switch (m) {
        case EditMode::MOVE:
            return QIcon(":none.svg");

        case EditMode::CREATE_POINT:
            return QIcon(":none.svg");

        case EditMode::REMOVE:
            return QIcon(":none.svg");
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

Section* modeSection(EditMode m) {
    switch (m) {
        case EditMode::MOVE:
            return Section::get("Special");

        case EditMode::CREATE_POINT:
            return Section::get("Point");

        case EditMode::REMOVE:
            return Section::get("Special");
    }
}

void initEditModes() {
    for (auto mode : {EditMode::MOVE, EditMode::CREATE_POINT, EditMode::REMOVE}) {
        modeSection(mode)->addMode(mode);
    }
}
