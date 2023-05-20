#include "EditMode.h"

EditMode EditMode::functionEditMode(EditMode::Type::FUNCTION, "", QIcon(), "");

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
