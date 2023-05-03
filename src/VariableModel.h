#pragma once

#include <QAbstractTableModel>
#include <QList>
#include <QString>

class RealGenerator;
class Geometry;

class VariableModel : public QAbstractTableModel {
    public:
        VariableModel(Geometry* geom, QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    private:
        Geometry* geom;
        QList<QString> names;
        QList<double> values;
};
