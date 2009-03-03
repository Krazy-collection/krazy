#include "includestreemodel.h"

IncludesTreeModel::IncludesTreeModel(CPlusPlus::Document::Ptr document)
  : m_rootDocument(document)
{
}

IncludesTreeModel::~IncludesTreeModel()
{
  m_rootDocument->release();
  m_rootDocument.clear();
}

int IncludesTreeModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant IncludesTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role == Qt::DisplayRole)
    return static_cast<CPlusPlus::Document*>(index.internalPointer())->fileName();

  return QVariant();
}

Qt::ItemFlags IncludesTreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant IncludesTreeModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const
{
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    return QVariant();

  switch (section)
  {
    case 0:
      return QString("Header");
    default:
      return QVariant();
  }
}

QModelIndex IncludesTreeModel::index(int row, int column, 
                                     const QModelIndex &parent) const
{
  if (!parent.isValid())
    return createIndex(0, 0, m_rootDocument.data());

  CPlusPlus::Document *parentObject;
  parentObject = static_cast<CPlusPlus::Document*>(parent.internalPointer());

  if (row >= 0 && row < parentObject->includes().count())
    return createIndex( row, column, parentObject->includes().at(row).document().data());
  else
    return QModelIndex();
}

QModelIndex IncludesTreeModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  CPlusPlus::Document *indexObject = static_cast<CPlusPlus::Document*>(index.internalPointer());
  if (indexObject == m_rootDocument.data())
    return QModelIndex();

  CPlusPlus::Document *parentObject = indexObject->parent().data();
  if (parentObject == m_rootDocument.data())
    return createIndex(0, 0, m_rootDocument.data());

  CPlusPlus::Document *grandParentObject = parentObject->parent().data();

  int idx = 0;
  foreach(CPlusPlus::Document::Include inc, grandParentObject->includes())
  {
    if (inc.document().data() == parentObject)
      break;
    idx++;
  }

  return createIndex(idx, 0, parentObject);
}

int IncludesTreeModel::rowCount(const QModelIndex &parent) const
{
  CPlusPlus::Document *parentObject;
  if (!parent.isValid())
    return 1;
  else
    parentObject = static_cast<CPlusPlus::Document*>(parent.internalPointer());

  return parentObject->includes().count();
}
