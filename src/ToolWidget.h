#pragma once

#include <QTabWidget>
#include <QHash>

class QAction;
class Function;
class EditMode;
class SectionMaster;

class ToolWidget : public QTabWidget {
    Q_OBJECT

    public:
        ToolWidget(
            const QHash<EditMode*, QAction*>& modeToAction,
            const QHash<Function*, QAction*>& funcToAction,
            const SectionMaster* sectionMaster,
            QWidget* parent = nullptr
        );

    private:
        void createUi(
            const QHash<EditMode*, QAction*>& modeToAction,
            const QHash<Function*, QAction*>& funcToAction,
            const SectionMaster* sectionMaster
        );

        static const int iconWidth = 50;
        static const int buttonWidth = 100;
};
