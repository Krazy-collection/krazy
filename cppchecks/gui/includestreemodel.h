#ifndef INCLUDES_TREE_MODEL_H
#define INCLUDES_TREE_MODEL_H

#include <cppmodel/document.h>

#include <QtGui/QStandardItemModel>

class QStandardItem;

class IncludesTreeModel :  public QStandardItemModel
{
  public: // Enum
    enum Role
    {
      DocumentRole = Qt::UserRole + 1
    };

  public:
    IncludesTreeModel(CPlusPlus::CppModel::Document::Ptr document);

  private: // Functions
    void addItems(CPlusPlus::CppModel::Document::Ptr document, QStandardItem *parent = 0);
};

#endif // INCLUDES_TREE_MODEL_H
