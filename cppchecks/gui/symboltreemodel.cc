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

#include "symboltreemodel.h"

#ifdef Q_CC_GNU
#include <cxxabi.h>
#endif

#include <cppmodel/overview.h>
#include <cppmodel/namespacebinding.h>

#include <parser/Scope.h>
#include <parser/Symbol.h>
#include <parser/Symbols.h>

#include <QtCore/QDebug>
#include <QtGui/QStandardItem>

using namespace CPlusPlus;
using namespace CppModel;

SymbolTreeModel::SymbolTreeModel(CPlusPlus::Symbol * const symbol)
  : m_namePrinter(new Overview())
{
  setHorizontalHeaderItem(0, new QStandardItem("Symbol Name"));
  setHorizontalHeaderItem(1, new QStandardItem("Symbol Type"));
  SymbolVisitor::accept(symbol);
}

SymbolTreeModel::SymbolTreeModel(NamespaceBinding * binding)
  : m_namePrinter(new Overview())
{
  setHorizontalHeaderItem(0, new QStandardItem("Symbol Name"));
  setHorizontalHeaderItem(1, new QStandardItem("Symbol Type"));

  accept(binding);
}

SymbolTreeModel::~SymbolTreeModel()
{}

/// Private functions

void SymbolTreeModel::accept(NamespaceBinding * binding)
{
  QStandardItem *item0 = new QStandardItem(m_namePrinter(binding->name()));
  QStandardItem *item1 = new QStandardItem("NamespaceBinding");

  QList<QStandardItem*> row;
  row.append(item0);
  row.append(item1);

  if (m_items.empty())
    invisibleRootItem()->appendRow(row);
  else
    m_items.top()->appendRow(row);

  // Push the item on the stack so that child items can use it as parent.
  m_items.push(item0);

  for (unsigned i = 0; i < binding->children.size(); ++i)
    accept(binding->children.at(i));

  for (unsigned i = 0; i < binding->symbols.size(); ++i)
  {
    Namespace *ns = binding->symbols[i];

    for (unsigned i = 0; i < ns->memberCount(); ++i)
    {
      if (!ns->memberAt(i)->isNamespace())
        SymbolVisitor::accept(ns->memberAt(i));
    }
  }

  if (!m_items.empty())
    m_items.pop();
}

/* virtual */ void SymbolTreeModel::postVisit(CPlusPlus::Symbol* symbol)
{
  Q_UNUSED(symbol);

  // The symbol and all its childs are processed so remove it from the stack.
  if (!m_items.empty())
    m_items.pop();
}

/* virtual */ bool SymbolTreeModel::preVisit(CPlusPlus::Symbol *symbol)
{
  QStandardItem *item0 = new QStandardItem(m_namePrinter(symbol->name()));
#ifdef Q_CC_GNU
  QStandardItem *item1 = new QStandardItem(abi::__cxa_demangle(
    typeid(*symbol).name(), 0, 0, 0) + 11);
#else
  QStandardItem *item1 = new QStandardItem(typeid(*symbol).name());
#endif

  // The first item in the row serves as parent for eventual childs of this
  // symbol.
  QList<QStandardItem*> row;
  row.append(item0);
  row.append(item1);

  if (m_items.empty())
    invisibleRootItem()->appendRow(row);
  else
    m_items.top()->appendRow(row);

  // Push the item on the stack so that child items can use it as parent.
  m_items.push(item0);

  return true;
}

///* virtual */ bool SymbolTreeModel::visit(CPlusPlus::Block *block)
//{
//  qDebug() << "Test: " << block->memberCount();
//  for (unsigned i = 0; i < block->memberCount(); ++i)
//  {
//    Symbol *member = block->memberAt(i);
//    qDebug() << abi::__cxa_demangle(typeid(*member).name(), 0, 0, 0) + 11;
//    accept(member);
//  }
//
//  return true;
//}
