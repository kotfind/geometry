#include "VariableModel.h"

#include "RealGenerator.h"
#include "Geometry.h"

VariableModel::VariableModel(Geometry* geom, QObject* parent)
  : QAbstractTableModel(parent),
    geom(geom),
    gens(geom->getRealGenerators())
{
    connect(
        geom,
        &Geometry::generatorRemoved,
        this,
        &VariableModel::onGeneratorRemoved
    );

    connect(
        geom,
        &Geometry::generatorChanged,
        this,
        &VariableModel::onGeneratorChanged
    );

    connect(
        geom,
        &Geometry::generatorMade,
        this,
        &VariableModel::onGeneratorMade
    );
}

int VariableModel::rowCount(const QModelIndex& parent) const {
    return gens.size();
}

int VariableModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QVariant VariableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    auto* gen = gens[index.row()];
    switch (index.column()) {
        case 0: return gen->getName();
        case 1: return gen->getValue();
    }
    return QVariant();
}

QVariant VariableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return QVariant();

    switch (section) {
        case 0: return tr("Name");
        case 1: return tr("Value");
    }
    return QVariant();
}

void VariableModel::onGeneratorRemoved(Generator* gen_) {
    if (!gen_->isReal()) return;
    auto* gen = static_cast<RealGenerator*>(gen_);

    int i = gens.indexOf(gen);

    beginRemoveRows(QModelIndex(), i, i);
    gens.remove(i);
    endRemoveRows();
}

void VariableModel::onGeneratorChanged(Generator* gen_) {
    if (!gen_->isReal()) return;
    auto* gen = static_cast<RealGenerator*>(gen_);

    int i = gens.indexOf(gen);
    emit dataChanged(index(i, 0), index(i, 1), { Qt::DisplayRole });
}

void VariableModel::onGeneratorMade(Generator* gen_) {
    if (!gen_->isReal()) return;
    auto* gen = static_cast<RealGenerator*>(gen_);

    int i = gens.size();

    beginInsertRows(QModelIndex(), i, i);
    gens << gen;
    endInsertRows();
}
