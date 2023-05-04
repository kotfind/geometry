#include "VariableDialog.h"

#include "VariableModel.h"

#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

VariableDialog::VariableDialog(QWidget* parent)
  : QDialog(parent)
{
    createUi();
}

void VariableDialog::setModel(VariableModel* model) {
    varCombo->setModel(varModel = model);
}

void VariableDialog::createUi() {
    auto* vbox = new QVBoxLayout(this);

    varCombo = new QComboBox(this);
    vbox->addWidget(varCombo);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    okButton = new QPushButton(tr("Ok"), this);
    hbox->addWidget(okButton);
    connect(
        okButton,
        &QPushButton::clicked,
        [this]() {
            accept();
        }
    );

    cancelButton = new QPushButton(tr("Cancel"), this);
    hbox->addWidget(okButton);
    connect(
        cancelButton,
        &QPushButton::clicked,
        [this]() {
            reject();
        }
    );
}

RealGenerator* VariableDialog::getCurrentVariable() const {
    int i = varCombo->currentIndex();
    if (i == -1) return nullptr;
    return varModel->getVariable(i);
}

RealGenerator* VariableDialog::getVariable(VariableModel* model, QWidget* parent) {
    VariableDialog d(parent);
    d.setModel(model);
    return d.exec() == QDialog::Accepted
        ? d.getCurrentVariable()
        : nullptr;
}
