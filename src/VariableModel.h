#pragma once

#include <QAbstractTableModel>
#include <QList>
#include <QString>

class Geometry;
class Generator;
class RealGenerator;

class VariableModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        VariableModel(Geometry* geom, QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    private:
        Geometry* geom;

        QList<RealGenerator*> gens;

    private slots:
        void onGeneratorRemoved(Generator*);
        void onGeneratorChanged(Generator*);
        void onGeneratorMade(Generator*);
};
