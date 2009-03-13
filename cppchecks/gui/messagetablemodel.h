#ifndef MESSAGE_TABLE_MODEL_H
#define MESSAGE_TABLE_MODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>

#include <cppmodel/diagnosticmessage.h>

class MessageTableModel :  public QAbstractTableModel
{
  public:
    MessageTableModel(QList<CPlusPlus::CppModel::DiagnosticMessage> const &messages);

    int columnCount(QModelIndex const &parent = QModelIndex()) const;

    QVariant data(QModelIndex const &index, int role) const;

    Qt::ItemFlags flags(QModelIndex const &index) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    int rowCount(QModelIndex const &parent = QModelIndex()) const;

  private:
    QList<CPlusPlus::CppModel::DiagnosticMessage> m_messages;
    int m_rowCount;
};

#endif // MESSAGE_TABLE_MODEL_H
