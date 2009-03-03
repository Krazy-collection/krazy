#ifndef INCLUDES_TREE_MODEL_H
#define INCLUDES_TREE_MODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>

#include <CppDocument.h>

class IncludesTreeModel :  public QAbstractItemModel
{
  public:
    IncludesTreeModel(CPlusPlus::Document::Ptr document);

    ~IncludesTreeModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

  private:
    CPlusPlus::Document::Ptr m_rootDocument;
};

#endif // INCLUDES_TREE_MODEL_H
