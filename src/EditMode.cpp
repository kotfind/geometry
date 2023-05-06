#include "EditMode.h"

EditMode* EditMode::modes[static_cast<int>(EditMode::Type::count)];

EditMode::EditMode(
    const QString& name,
    const QIcon& icon,
    const QString& description,
    Type type
) : name(name),
    icon(icon),
    description(description),
    type(type)
{}

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
