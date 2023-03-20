#pragma once

#include "Type.h"

#include <QString>

class ArgumentInfo {
    public:
        ArgumentInfo(Type type, const QString& description);

        const QString& getDescription() const { return description; }
        Type getType() const { return type; }

    private:
        const Type type;
        const QString description;
};
