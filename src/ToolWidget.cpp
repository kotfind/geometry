#include "ToolWidget.h"

#include "Section.h"

#include <QList>
#include <QVBoxLayout>
#include <QToolButton>

ToolWidget::ToolWidget(
    const QHash<EditMode, QAction*>& modeToAction,
    const QHash<Function*, QAction*>& funcToAction,
    QWidget* parent
) : QTabWidget(parent)
{
    createUi(modeToAction, funcToAction);
}

void ToolWidget::createUi(
    const QHash<EditMode, QAction*>& modeToAction,
    const QHash<Function*, QAction*>& funcToAction
) {
    for (auto* section : Section::getMaster()->getSections()) {
        QList<QAction*> actions;
        for (auto mode : section->getModes()) {
            actions << modeToAction[mode];
        }
        for (auto* func : section->getFunctions()) {
            actions << funcToAction[func];
        }

        auto* tab = new QWidget(this);
        addTab(tab, section->getName());

        auto* layout = new QVBoxLayout(this);
        tab->setLayout(layout);

        for (auto* action : actions) {
            auto* btn = new QToolButton(this);
            btn->setDefaultAction(action);
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            layout->addWidget(btn);
        }
    }
}
