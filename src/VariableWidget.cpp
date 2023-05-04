#include "VariableWidget.h"

#include "VariableModel.h"

#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

VariableWidget::VariableWidget(QWidget* parent)
  : QWidget(parent)
{
    createUi();
}

void VariableWidget::setModel(VariableModel* model) {
    view->setModel(model);
}

void VariableWidget::createUi() {
    auto* vbox = new QVBoxLayout;
    setLayout(vbox);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    insertButton = new QPushButton(tr("+"), this);
    hbox->addWidget(insertButton);
    connect(
        insertButton,
        &QPushButton::clicked,
        this,
        &VariableWidget::onInsertClicked
    );

    removeButton = new QPushButton(tr("-"), this);
    hbox->addWidget(removeButton);
    connect(
        removeButton,
        &QPushButton::clicked,
        this,
        &VariableWidget::onRemoveClicked
    );

    hbox->addStretch(1);

    view = new QTableView(this);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    vbox->addWidget(view);
}

void VariableWidget::onInsertClicked() {
    auto* model = view->model();
    model->insertRow(model->rowCount());
}

void VariableWidget::onRemoveClicked() {
    const auto index = view->currentIndex();
    if (!index.isValid()) return;

    view->model()->removeRow(index.row());
}
