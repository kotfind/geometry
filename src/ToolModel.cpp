#include "ToolModel.h"

#include "Section.h"
#include "Function.h"
#include "EditMode.h"

#include <QString>
#include <QList>
#include <cassert>

// -------------------- ToolModelItemType --------------------

enum class ToolModelItemType {
    FUNC,
    SECT,
    MODE,
};

// -------------------- ToolModelItem --------------------

class ToolModelItem {
    public:
        ToolModelItem();
        ToolModelItem(Function*, ToolModelItem* parent);
        ToolModelItem(Section*, ToolModelItem* parent);
        ToolModelItem(EditMode, ToolModelItem* parent);

        ~ToolModelItem();

        int getRow() const;
        QString getName() const;

        ToolModelItemType type;
        Function* func = nullptr;
        Section* sec = nullptr;
        EditMode mode = EditMode::MOVE;

        ToolModelItem* parent = nullptr;

        QList<ToolModelItem*> children;
};

ToolModelItem::ToolModelItem() {
    auto* master = Section::getMaster();
    new ToolModelItem(EditMode::MOVE, this);
    new ToolModelItem(EditMode::CREATE_POINT, this);
    new ToolModelItem(EditMode::REMOVE, this);
    for (auto* sec : master->getSections()) {
        new ToolModelItem(sec, this);
    }
    for (auto* func : master->getFunctions()) {
        new ToolModelItem(func, this);
    }
}

ToolModelItem::ToolModelItem(Function* func, ToolModelItem* parent)
  : parent(parent),
    type(ToolModelItemType::FUNC),
    func(func)
{
    parent->children << this;
}

ToolModelItem::ToolModelItem(Section* sec, ToolModelItem* parent)
  : parent(parent),
    type(ToolModelItemType::SECT),
    sec(sec)
{
    parent->children << this;
    for (auto* s : sec->getSections()) {
        new ToolModelItem(s, this);
    }
    for (auto* f : sec->getFunctions()) {
        new ToolModelItem(f, this);
    }
}

ToolModelItem::ToolModelItem(EditMode mode, ToolModelItem* parent)
  : parent(parent),
    type(ToolModelItemType::MODE),
    mode(mode)
{
    parent->children << this;
}

ToolModelItem::~ToolModelItem() {
    for (auto* child : children) {
        delete child;
    }
}

int ToolModelItem::getRow() const {
    assert(parent);
    return parent->children.indexOf(this);
}

QString ToolModelItem::getName() const {
    switch (type) {
        case ToolModelItemType::FUNC: return func->getSelfName();
        case ToolModelItemType::SECT: return sec->getName();
        case ToolModelItemType::MODE: return modeName(mode);
    }
}

// -------------------- ToolModel --------------------

ToolModel::ToolModel(QObject* parent)
  : QAbstractItemModel(parent),
    root(new ToolModelItem)
{}

ToolModel::~ToolModel() {
    delete root;
}

QModelIndex ToolModel::parent(const QModelIndex& index) const {
    if (!index.isValid()) return QModelIndex();

    auto* item = static_cast<ToolModelItem*>(index.internalPointer());
    auto* parent = item->parent;
    return parent == root ? QModelIndex() : createIndex(parent->getRow(), 0, parent);
}

QModelIndex ToolModel::index(int row, int column, const QModelIndex& parentIndex) const {
    if (!hasIndex(row, column, parentIndex))
        return QModelIndex();

    auto* parent = parentIndex.isValid()
        ? static_cast<ToolModelItem*>(parentIndex.internalPointer())
        : root;
    if (0 <= row && row < parent->children.size()) {
        auto* item = parent->children[row];
        return createIndex(row, column, item);
    } else {
        return QModelIndex();
    }
}

int ToolModel::rowCount(const QModelIndex& parentIndex) const {
    if (parentIndex.isValid()) {
        auto* parent = static_cast<ToolModelItem*>(parentIndex.internalPointer());
        return parent->children.size();
    } else {
        return root->children.size();
    }
}

int ToolModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant ToolModel::data(const QModelIndex& index, int role) const {
    if (role != Qt::DisplayRole || !index.isValid() || index.column() != 0)
        return QVariant();

    auto* item = static_cast<ToolModelItem*>(index.internalPointer());
    return item->getName();
}

QVariant ToolModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}
