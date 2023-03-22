#pragma once

#include "EditMode.h"

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
        EditMode mode = EditMode::MOVE;

        QLabel* nameLabel;
        QLabel* descriptionLabel;

        QTreeView* argsView;
        ArgumentInfoModel* argsModel;

    public slots:
        void setFunction(Function* func = nullptr);
        void setMode(EditMode);
};
