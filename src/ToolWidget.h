#pragma once

#include <QTabWidget>
#include <QHash>

class QAction;
class Function;
enum class EditMode;

class ToolWidget : public QTabWidget {
    Q_OBJECT

    public:
        ToolWidget(
            const QHash<EditMode, QAction*>& modeToAction,
            const QHash<Function*, QAction*>& funcToAction,
            QWidget* parent = nullptr
        );

    private:
        void createUi(
            const QHash<EditMode, QAction*>& modeToAction,
            const QHash<Function*, QAction*>& funcToAction
        );
};
