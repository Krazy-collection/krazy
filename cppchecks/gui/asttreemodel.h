#ifndef ASTTREEMODEL_H
#define ASTTREEMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>
#include <CPlusPlusForwardDeclarations.h>

class Item;

class ASTTreeModel : public QAbstractItemModel
{
  public:
    ASTTreeModel();
    ~ASTTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Item* getRootItem();

  private:
    void setupModelData(QString fileName, Item *parent, QString filterText);

    Item *rootItem;
};

#endif // ASTTREEMODEL_H
