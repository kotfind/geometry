#include "ArgumentInfoModel.h"

#include "Function.h"
#include "ArgumentInfo.h"
#include "Type.h"

#include <QStringList>

ArgumentInfoModel::ArgumentInfoModel(QObject* parent)
  : QAbstractListModel(parent)
{}

int ArgumentInfoModel::rowCount(const QModelIndex& parent) const {
    return func ? func->countArgs() : 0;
}

int ArgumentInfoModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QVariant ArgumentInfoModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    const auto& argInfo = func->getArgInfo(index.row());
    switch (index.column()) {
        case 0: return complexTypeName(argInfo.getType()).join(" or ");
        case 1: return argInfo.getDescription();
        default: return QVariant();
    }
}

QVariant ArgumentInfoModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return QVariant();

    switch (section) {
        case 0: return tr("Type");
        case 1: return tr("Description");
        default: return QVariant();
    }
}

void ArgumentInfoModel::setFunction(Function* f) {
    beginResetModel();
    func = f;
    endResetModel();
}
