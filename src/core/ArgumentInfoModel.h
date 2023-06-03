#pragma once

#include <QAbstractListModel>

class Function;
class AbstractGeometry;

class ArgumentInfoModel : public QAbstractListModel {
    Q_OBJECT

    public:
        ArgumentInfoModel(const AbstractGeometry* geom, QObject* parent = nullptr);

        void setActiveGeometry(const AbstractGeometry* geom);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    private:
        const Function* func = nullptr;

        int selectedCount = 0;

        const AbstractGeometry* geom;

    public slots:
        void setFunction(const Function*);
        void updateSelectedCount(int n);
};
