#ifndef MESSAGE_TABLE_MODEL_H
#define MESSAGE_TABLE_MODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>

#include <CppDocument.h>

class MessageTableModel :  public QAbstractTableModel
{
  public:
    MessageTableModel(QList<CPlusPlus::Document::DiagnosticMessage> const &messages);

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

  private:
    QList<CPlusPlus::Document::DiagnosticMessage> m_messages;
    int m_rowCount;
};

#endif // MESSAGE_TABLE_MODEL_H
