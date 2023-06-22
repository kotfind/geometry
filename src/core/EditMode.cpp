#include "EditMode.h"

#include "util/TR.h"

EditMode* EditMode::modes[static_cast<int>(EditMode::Type::count)];

EditMode::EditMode(
    Type type,
    const QString& name,
    const QIcon& icon,
    const QString& description
) : type(type),
    name(name),
    icon(icon),
    description(description)
{
    modes[static_cast<int>(type)] = this;
}

const QString& EditMode::getName() const {
    return name;
}

const QIcon& EditMode::getIcon() const {
    return icon;
}

const QString& EditMode::getDescription() const {
    return description;
}

EditMode::Type EditMode::getType() const {
    return type;
}

EditMode* EditMode::get(Type t) {
    return modes[static_cast<int>(t)];
}

void EditMode::init() {
    new EditMode(
        EditMode::Type::MOVE,
        TR("Move"),
        QIcon(QIcon(":move.svg")),
        TR(
            "Drag any free object with mouse to move it. "
            "Use middle mouse button to move around scene."
        )
    );

    new EditMode(
        EditMode::Type::CREATE_POINT,
        TR("Create"),
        QIcon(QIcon(":create_point.svg")),
        TR("Click on the canvas to create a point.")
    );

    new EditMode(
        EditMode::Type::REMOVE,
        TR("Remove"),
        QIcon(QIcon(":remove.svg")),
        TR("Click on any object to delete it.")
    );

    new EditMode(
        EditMode::Type::FUNCTION,
        "",
        QIcon(),
        ""
    );

    new EditMode(
        EditMode::Type::HIDE,
        TR("Hide/Show"),
        QIcon(QIcon(":hide.svg")),
        TR("Click on any object to toggle it's visibility.")
    );
}

void EditMode::cleanup() {
    for (int i = 0; i < static_cast<int>(Type::count); ++i) {
        delete modes[i];
    }
}
