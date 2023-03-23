#pragma once

#include <QAbstractItemModel>

class ToolModelItem;

class ToolModel : public QAbstractItemModel {
    public:
        ToolModel(QObject* parent = nullptr);

        QModelIndex parent(const QModelIndex& index) const override;
        QModelIndex index(int row, int column, const QModelIndex& parent) const override;

        int rowCount(const QModelIndex& parent) const override;
        int columnCount(const QModelIndex& parent) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    private:
        ToolModelItem* root = nullptr;
};
