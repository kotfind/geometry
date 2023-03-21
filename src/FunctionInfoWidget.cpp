#include "FunctionInfoWidget.h"

#include "ArgumentInfoModel.h"
#include "Function.h"

#include <QVBoxLayout>
#include <QTreeView>
#include <QLabel>

FunctionInfoWidget::FunctionInfoWidget(QWidget* parent) : QWidget(parent) {
    setEnabled(false);
    createUi();

    argsModel = new ArgumentInfoModel(this);
    argsView->setModel(argsModel);
}

void FunctionInfoWidget::createUi() {
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    nameLabel = new QLabel(this);
    nameLabel->setStyleSheet("font-weight: bold;");
    vbox->addWidget(nameLabel, 0, Qt::AlignCenter);

    descriptionLabel = new QLabel(this);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(descriptionLabel, 0);

    argsView = new QTreeView(this);
    vbox->addWidget(argsView, 0);

    vbox->addStretch(1);
}

void FunctionInfoWidget::setFunction(Function* f) {
    func = f;
    argsModel->setFunction(func);
    if (func) {
        nameLabel->setText(f->getName());
        descriptionLabel->setText(f->getDescription());
        setEnabled(true);
    } else {
        nameLabel->setText("");
        descriptionLabel->setText("");
        setEnabled(false);
    }
}
