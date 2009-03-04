#include "asttreemodel.h"

#ifdef Q_OS_LINUX
#include <cxxabi.h>
#endif

#include "astitem.h"

ASTTreeModel::ASTTreeModel()
{
    rootItem = new Item();
}

ASTTreeModel::~ASTTreeModel()
{
  delete rootItem;
}

Item* ASTTreeModel::getRootItem()
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

     Item *item = static_cast<Item*>(index.internalPointer());

     if (0 == index.column())
#ifdef Q_OS_LINUX
       return QString(abi::__cxa_demangle(typeid(*item->ast()).name(), 0, 0, 0) + 11 );
#else
       return typeid(*item->ast()).name();
#endif

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

     Item *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<Item*>(parent.internalPointer());

     Item *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

 QModelIndex ASTTreeModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     Item *childItem = static_cast<Item*>(index.internalPointer());
     Item *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }

 int ASTTreeModel::rowCount(const QModelIndex &parent) const
 {
     Item *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<Item*>(parent.internalPointer());

     return parentItem->childCount();
 }

 int ASTTreeModel::columnCount(const QModelIndex &parent) const
 {
     if (parent.isValid())
         return static_cast<Item*>(parent.internalPointer())->columnCount();
     else
         return rootItem->columnCount();
 }

 bool ASTTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
 {
     return QAbstractItemModel::setData(index, value, role | Qt::CheckStateRole);
 }
