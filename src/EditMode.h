#pragma once

class QString;

enum class EditMode {
    MOVE,
    CREATE_POINT,
    REMOVE,
    FUNCTION
};

QString modeName(EditMode);
QString modeDescription(EditMode);
