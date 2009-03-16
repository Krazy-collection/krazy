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

#include "asttreemodel.h"

#ifdef Q_CC_GNU
#include <cxxabi.h>
#endif

#include <parser/AST.h>
#include <parser/TranslationUnit.h>

#include <QtCore/QDebug>
#include <QtGui/QStandardItem>

using namespace CPlusPlus;

ASTTreeModel::ASTTreeModel(TranslationUnit *translationUnit)
  : ASTVisitor(translationUnit->control())
{
  setHorizontalHeaderItem(0, new QStandardItem("AST Node Type"));
  accept(translationUnit->ast());
}

ASTTreeModel::~ASTTreeModel()
{}

/* virtual */ void ASTTreeModel::postVisit(CPlusPlus::AST* ast)
{
  if (!m_items.empty())
    m_items.pop();
}

/* virtual */ bool ASTTreeModel::preVisit(CPlusPlus::AST *ast)
{
#ifdef Q_CC_GNU
  QStandardItem *item = new QStandardItem(abi::__cxa_demangle(
    typeid(*ast).name(), 0, 0, 0) + 11);
#else
  QStandardItem *item = new QStandardItem((ast).name());
#endif

  //item->setData(var, DocumentRole); // NOTE: additional information can be added here.

  if (m_items.empty())
    invisibleRootItem()->appendRow(item);
  else
    m_items.top()->appendRow(item);

  m_items.push(item);

  return true;
}
