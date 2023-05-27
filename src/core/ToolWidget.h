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
        ToolWidget(QWidget* parent = nullptr);

        void update(
            const QHash<const EditMode*, QAction*>& modeToAction,
            const QHash<const Function*, QAction*>& funcToAction,
            const SectionMaster* sectionMaster
        );

    private:
        static const int iconWidth = 50;
        static const int buttonWidth = 100;
};
