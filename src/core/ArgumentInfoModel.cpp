#include "ArgumentInfoModel.h"

#include "Function.h"
#include "ArgumentInfo.h"
#include "AbstractGeometry.h"

#include <QColor>
#include <QStringList>

ArgumentInfoModel::ArgumentInfoModel(const AbstractGeometry* geom, QObject* parent)
  : QAbstractListModel(parent),
    geom(geom)
{}

int ArgumentInfoModel::rowCount(const QModelIndex& parent) const {
    return func ? func->countArgs() : 0;
}

int ArgumentInfoModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QVariant ArgumentInfoModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return QVariant();

    if (role == Qt::DisplayRole) {
        const auto& argInfo = func->getArgInfo(index.row());
        switch (index.column()) {
            case 0: return geom->complexTypeName(argInfo.getType()).join(" or ");
            case 1: return argInfo.getDescription();
        }
    } else if (role == Qt::BackgroundRole) {
        if (index.row() < selectedCount) {
            return QColor(Qt::lightGray);
        } else if (index.row() == selectedCount) {
            return QColor(Qt::green);
        }
    }
    return QVariant();
}

QVariant ArgumentInfoModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return QVariant();

    switch (section) {
        case 0: return tr("Type");
        case 1: return tr("Description");
        default: return QVariant();
    }
}

void ArgumentInfoModel::setFunction(const Function* f) {
    beginResetModel();
    func = f;
    endResetModel();
}

void ArgumentInfoModel::updateSelectedCount(int n) {
    selectedCount = n;
    dataChanged(
        index(0, 0),
        index(rowCount(), columnCount()),
        {Qt::BackgroundRole}
    );
}
