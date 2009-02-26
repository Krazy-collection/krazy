#include "asttreemodel.h"

#include "asttreeitem.h"

ASTTreeModel::ASTTreeModel()
{
    rootItem = new ASTTreeItem();
}

ASTTreeItem* ASTTreeModel::getRootItem()
{
    return rootItem;
}

QVariant ASTTreeModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();

     if (role == Qt::CheckStateRole)
         return Qt::Checked;

     ASTTreeItem *item = static_cast<ASTTreeItem*>(index.internalPointer());


    return item->data(index.column());
 }

 Qt::ItemFlags ASTTreeModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
 }

 QVariant ASTTreeModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
         return rootItem->data(section);

     return QVariant();
 }

 QModelIndex ASTTreeModel::index(int row, int column, const QModelIndex &parent)
             const
 {
     if (!hasIndex(row, column, parent))
         return QModelIndex();

     ASTTreeItem *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<ASTTreeItem*>(parent.internalPointer());

     ASTTreeItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

 QModelIndex ASTTreeModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     ASTTreeItem *childItem = static_cast<ASTTreeItem*>(index.internalPointer());
     ASTTreeItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }

 int ASTTreeModel::rowCount(const QModelIndex &parent) const
 {
     ASTTreeItem *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<ASTTreeItem*>(parent.internalPointer());

     return parentItem->childCount();
 }

  ASTTreeModel::~ASTTreeModel()
 {
     delete rootItem;
 }

 int ASTTreeModel::columnCount(const QModelIndex &parent) const
 {
     if (parent.isValid())
         return static_cast<ASTTreeItem*>(parent.internalPointer())->columnCount();
     else
         return rootItem->columnCount();
 }

 bool ASTTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
 {
     return QAbstractItemModel::setData(index, value, role | Qt::CheckStateRole);
 }
