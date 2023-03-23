#include "InstrumentInfoWidget.h"

#include "ArgumentInfoModel.h"
#include "Function.h"

#include <QVBoxLayout>
#include <QTreeView>
#include <QLabel>
#include <cassert>
#include <QSizePolicy>

InstrumentInfoWidget::InstrumentInfoWidget(QWidget* parent) : QWidget(parent) {
    setEnabled(false);
    createUi();

    argsModel = new ArgumentInfoModel(this);
    argsView->setModel(argsModel);
    argsView->setSelectionMode(QTreeView::SelectionMode::NoSelection);
}

void InstrumentInfoWidget::createUi() {
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

void InstrumentInfoWidget::setFunction(Function* f) {
    assert(mode == EditMode::FUNCTION);
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

void InstrumentInfoWidget::setMode(EditMode m) {
    mode = m;
    if (mode == EditMode::FUNCTION) {
        setFunction();
        argsView->setVisible(true);
    } else {
        nameLabel->setText(modeName(m));
        descriptionLabel->setText(modeDescription(m));
        argsView->setVisible(false);
        setEnabled(true);
    }
}

void InstrumentInfoWidget::updateSelectedCount(int n) {
    argsModel->updateSelectedCount(n);
}
