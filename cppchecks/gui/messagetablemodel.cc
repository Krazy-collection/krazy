#include "messagetablemodel.h"

using namespace CPlusPlus;
using namespace CppModel;

MessageTableModel::MessageTableModel(QList<DiagnosticMessage> const &messages)
  : m_messages(messages)
{}

int MessageTableModel::columnCount(QModelIndex const &parent) const
{
  return 2;
}

QVariant MessageTableModel::data(QModelIndex const &index, int role) const
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

Qt::ItemFlags MessageTableModel::flags(QModelIndex const &index) const
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

int MessageTableModel::rowCount(QModelIndex const &parent) const
{
  return m_messages.size();
}
