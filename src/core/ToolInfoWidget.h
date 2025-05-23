#pragma once

#include <QWidget>

class Function;
class QLabel;
class QTreeView;
class ArgumentInfoModel;
class QSize;
class EditMode;
class AbstractGeometry;

class ToolInfoWidget : public QWidget {
    Q_OBJECT

    public:
        ToolInfoWidget(const AbstractGeometry* geom, QWidget* parent = nullptr);

        void setActiveGeometry(const AbstractGeometry* geom);

    private:
        void createUi();

        const Function* func = nullptr;
        const EditMode* mode = nullptr;

        QLabel* nameLabel;
        QLabel* descriptionLabel;

        QTreeView* argsView;
        ArgumentInfoModel* argsModel;

    public slots:
        void setFunction(const Function* func = nullptr);
        void setMode(const EditMode*);
        void updateSelectedCount(int n);
};
