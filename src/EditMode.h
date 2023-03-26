#pragma once

class QString;
class Section;
class QIcon;

enum class EditMode {
    MOVE,
    CREATE_POINT,
    REMOVE,
    FUNCTION
};

QString modeName(EditMode);
QIcon modeIcon(EditMode);
QString modeDescription(EditMode);
Section* modeSection(EditMode);

void initEditModes();
