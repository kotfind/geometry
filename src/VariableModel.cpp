#include "VariableModel.h"

#include "RealGenerator.h"
#include "Geometry.h"
#include "Real.h"

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

    connect(
        geom,
        &Geometry::resetCompleted,
        this,
        &VariableModel::onGeometryReset
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

void VariableModel::onGeometryReset() {
    beginResetModel();
    gens = geom->getRealGenerators();
    endResetModel();
}

bool VariableModel::setData(const QModelIndex& index, const QVariant& data, int role) {
    if (role != Qt::EditRole) return false;

    auto* gen = gens[index.row()];
    switch (index.column()) {
        case 0: {
            auto name = data.toString();
            if (name.contains(' ')) return false;
            gen->setName(name);
        } break;

        case 1: {
            bool ok;
            auto val = data.toDouble(&ok);
            if (!ok) return false;
            gen->setValue(val);
        } break;

        default:
            return false;
    }

    return true;
}

Qt::ItemFlags VariableModel::flags(const QModelIndex& index) const {
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool VariableModel::insertRows(int row, int count, const QModelIndex& parent) {
    if (count != 1 || row != rowCount()) return false;

    geom->makeRealGenerator("new", std::make_unique<Real>(0));

    return true;
}

bool VariableModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (count != 1) return false;

    geom->removeGenerator(gens[row]);

    return true;
}

RealGenerator* VariableModel::getVariable(int row) const {
    return gens[row];
}
