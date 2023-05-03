#include "VariableModel.h"

#include "RealGenerator.h"
#include "Geometry.h"

VariableModel::VariableModel(Geometry* geom, QObject* parent)
  : QAbstractTableModel(parent),
    geom(geom)
{
    const auto& gens = geom->getRealGenerators();
    names.reserve(gens.size());
    values.reserve(gens.size());
    for (auto* gen : gens) {
        names << gen->getName();
        values << gen->getValue();
    }
}

int VariableModel::rowCount(const QModelIndex& parent) const {
    return names.size();
}

int VariableModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QVariant VariableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    int i = index.row();
    switch (index.column()) {
        case 0: return names[i];
        case 1: return values[i];
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
