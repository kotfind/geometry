#pragma once

#include <QWidget>

class Function;
class QLabel;
class QTreeView;
class ArgumentInfoModel;

class FunctionInfoWidget : public QWidget {
    Q_OBJECT

    public:
        FunctionInfoWidget(QWidget* parent = nullptr);

    private:
        void createUi();

        Function* func = nullptr;

        QLabel* nameLabel;
        QLabel* descriptionLabel;

        QTreeView* argsView;
        ArgumentInfoModel* argsModel;

    public slots:
        void setFunction(Function* func = nullptr);
};
