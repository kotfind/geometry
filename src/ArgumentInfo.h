#pragma once

#include "Object.h"

#include <QString>

class ArgumentInfo {
    public:
        ArgumentInfo(Object::Type type, const QString& description);

        const QString& getDescription() const { return description; }
        Object::Type getType() const { return type; }

    private:
        const Object::Type type;
        const QString description;
};
