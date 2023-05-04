#pragma once

#include <QWidget>

class QTableView;
class QPushButton;
class VariableModel;

class VariableWidget : public QWidget {
    Q_OBJECT

    public:
        VariableWidget(QWidget* parent = nullptr);

        void setModel(VariableModel*);

    private:
        void createUi();

        QPushButton* insertButton;
        QPushButton* removeButton;

        QTableView* view;

    private slots:
        void onInsertClicked();
        void onRemoveClicked();
};
