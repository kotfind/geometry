#include "ToolWidget.h"

#include "Section.h"

#include <QList>
#include <QGridLayout>
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

        auto* layout = new QGridLayout(this);
        tab->setLayout(layout);

        for (int i = 0; i < actions.size(); ++i) {
            auto* btn = new QToolButton(this);

            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            btn->setFixedWidth(buttonWidth);
            btn->setIconSize(QSize(iconWidth, iconWidth));
            btn->setAutoRaise(true);

            btn->setDefaultAction(actions[i]);
            layout->addWidget(
                btn,
                i / 3,
                i % 3,
                Qt::AlignTop
            );
        }

        layout->setRowStretch(actions.size() / 3, 1);
    }
}
