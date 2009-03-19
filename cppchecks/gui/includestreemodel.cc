/*
    Copyright (C) 2009 Bertjan Broeksema <b.broeksema@kdemail.net>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

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
    item->setToolTip(inc.document()->absoluteFileName());

    parent->appendRow(item);
    addItems(inc.document(),item);
  }
}
