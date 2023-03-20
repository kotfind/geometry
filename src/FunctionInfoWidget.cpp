#include "FunctionInfoWidget.h"

#include "Function.h"

#include <QVBoxLayout>
#include <QLabel>

FunctionInfoWidget::FunctionInfoWidget(QWidget* parent) : QWidget(parent) {
    createUi();
    setEnabled(false);
}

void FunctionInfoWidget::createUi() {
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    nameLabel = new QLabel(this);
    nameLabel->setStyleSheet("font-weight: bold;");
    vbox->addWidget(nameLabel, 0, Qt::AlignCenter);

    descriptionLabel = new QLabel(this);
    descriptionLabel->setWordWrap(true);
    vbox->addWidget(descriptionLabel, 0);

    vbox->addStretch(1);
}

void FunctionInfoWidget::setFunction(Function* f) {
    func = f;
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
