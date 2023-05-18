#pragma once

#include "Object.h"

#include <QString>

class ArgumentInfo {
    public:
        ArgumentInfo(int type, const QString& description);

        const QString& getDescription() const { return description; }
        int getType() const { return type; }

    private:
        const int type;
        const QString description;
};
