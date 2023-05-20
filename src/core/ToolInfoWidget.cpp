#include "ToolInfoWidget.h"

#include "ArgumentInfoModel.h"
#include "Function.h"
#include "EditMode.h"

#include <QVBoxLayout>
#include <QTreeView>
#include <QLabel>
#include <cassert>
#include <QSize>

ToolInfoWidget::ToolInfoWidget(const AbstractGeometry* geom, QWidget* parent)
  : QWidget(parent)
{
    setEnabled(false);
    createUi();

    argsModel = new ArgumentInfoModel(geom, this);
    argsView->setModel(argsModel);
    argsView->setSelectionMode(QTreeView::SelectionMode::NoSelection);
}

void ToolInfoWidget::createUi() {
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    nameLabel = new QLabel(this);
    nameLabel->setStyleSheet("font-weight: bold;");
    vbox->addWidget(nameLabel, 0, Qt::AlignCenter);

    descriptionLabel = new QLabel(this);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setFrameShape(QFrame::StyledPanel);
    descriptionLabel->setAlignment(Qt::AlignJustify);
    descriptionLabel->setMargin(5);
    vbox->addWidget(descriptionLabel, 0);

    argsView = new QTreeView(this);
    auto policy = argsView->sizePolicy();
    policy.setRetainSizeWhenHidden(true);
    argsView->setSizePolicy(policy);
    vbox->addWidget(argsView, 0);

    vbox->addStretch(1);
}

void ToolInfoWidget::setFunction(const Function* f) {
    assert(mode->getType() == EditMode::Type::FUNCTION);
    func = f;
    argsModel->setFunction(func);
    if (func) {
        nameLabel->setText(f->getFullName());
        descriptionLabel->setText(f->getDescription());
        setEnabled(true);
    } else {
        nameLabel->setText("");
        descriptionLabel->setText("");
        setEnabled(false);
    }
}

void ToolInfoWidget::setMode(const EditMode* m) {
    mode = m;
    if (mode->getType() == EditMode::Type::FUNCTION) {
        setFunction();
        argsView->setVisible(true);
    } else {
        nameLabel->setText(m->getName());
        descriptionLabel->setText(m->getDescription());
        argsView->setVisible(false);
        setEnabled(true);
    }
}

void ToolInfoWidget::updateSelectedCount(int n) {
    argsModel->updateSelectedCount(n);
}
