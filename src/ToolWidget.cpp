#include "ToolWidget.h"

#include "SectionMaster.h"
#include "Section.h"

#include <QList>
#include <QGridLayout>
#include <QToolButton>

ToolWidget::ToolWidget(
    const QHash<const EditMode*, QAction*>& modeToAction,
    const QHash<const Function*, QAction*>& funcToAction,
    const SectionMaster* sectionMaster,
    QWidget* parent
) : QTabWidget(parent)
{
    createUi(modeToAction, funcToAction, sectionMaster);
}

void ToolWidget::createUi(
    const QHash<const EditMode*, QAction*>& modeToAction,
    const QHash<const Function*, QAction*>& funcToAction,
    const SectionMaster* sectionMaster
) {
    for (auto* section : sectionMaster->getSections()) {
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
