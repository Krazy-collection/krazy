#include "messagetablemodel.h"

MessageTableModel::MessageTableModel(QList<CPlusPlus::Document::DiagnosticMessage> const &messages)
  : m_messages(messages)
{
}

int MessageTableModel::columnCount(const QModelIndex &parent) const
{
  return 2;
}

QVariant MessageTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role == Qt::DisplayRole)
  {
    switch (index.column())
    {
      case 0:
        return m_messages.at(index.row()).line();
      default:
        return m_messages.at(index.row()).text();
    }
  }

  return QVariant();
}

Qt::ItemFlags MessageTableModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant MessageTableModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const
{
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    return QVariant();

  switch (section)
  {
    case 0:
      return QString("Line");
    case 1:
      return QString("Message");
    default:
      return QVariant();
  }
}

int MessageTableModel::rowCount(const QModelIndex &parent) const
{
  return m_messages.size();
}
