#include "includestreemodel.h"

#include <QtGui/QStandardItem>

using namespace CPlusPlus;
using namespace CppModel;

IncludesTreeModel::IncludesTreeModel(Document::Ptr document)
{
  setHorizontalHeaderItem(0, new QStandardItem("File name"));

  QVariant var;
  var.setValue<Document::Ptr>(document);

  QStandardItem *item = new QStandardItem(document->fileName());
  item->setData(var, DocumentRole);
  invisibleRootItem()->appendRow(item);

  addItems(document, item);
}

void IncludesTreeModel::addItems(Document::Ptr document, QStandardItem *parent)
{
  foreach (Document::Include inc, document->includes())
  {
    QString name;
    if (inc.type() == Client::IncludeLocal)
      name = '"' + inc.document()->fileName() + '"';
    else
      name = '<' + inc.document()->fileName() + '>';

    QVariant var;
    var.setValue<Document::Ptr>(inc.document());

    QStandardItem *item = new QStandardItem(name);
    item->setData(var, DocumentRole);

    parent->appendRow(item);
    addItems(inc.document(),item);
  }
}
