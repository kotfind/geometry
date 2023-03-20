#pragma once

#include <QWidget>

class Function;
class QLabel;

class FunctionInfoWidget : public QWidget {
    Q_OBJECT

    public:
        FunctionInfoWidget(QWidget* parent = nullptr);

    private:
        void createUi();

        Function* func = nullptr;

        QLabel* nameLabel;
        QLabel* descriptionLabel;

    public slots:
        void setFunction(Function* func = nullptr);
};
