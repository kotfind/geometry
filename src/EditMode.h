#pragma once

class QString;
class Section;

enum class EditMode {
    MOVE,
    CREATE_POINT,
    REMOVE,
    FUNCTION
};

QString modeName(EditMode);
QString modeDescription(EditMode);
Section* modeSection(EditMode);

void initEditModes();
