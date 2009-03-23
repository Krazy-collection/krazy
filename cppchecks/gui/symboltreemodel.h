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
#ifndef SYMBOL_TREEMODEL_H
#define SYMBOL_TREEMODEL_H

#include <cppmodel/nameprettyprinter.h>

#include <parser/CPlusPlusForwardDeclarations.h>
#include <parser/SymbolVisitor.h>

#include <QtCore/QStack>
#include <QtGui/QStandardItemModel>

namespace CPlusPlus {
  namespace CppModel {
    class NamespaceBinding;
  }
}


class SymbolTreeModel : public QStandardItemModel, private CPlusPlus::SymbolVisitor
{
  public: // Functions
    /**
     * Creates a TreeModel for given Scope. This class will not take
     * ownership of @param scope and you should make shure that this
     * model is not used after deletion of @param scope.
     */
    SymbolTreeModel(CPlusPlus::Symbol * const symbol);

    SymbolTreeModel(CPlusPlus::CppModel::NamespaceBinding * binding);

    ~SymbolTreeModel();

  private: // Functions
    void accept(CPlusPlus::CppModel::NamespaceBinding * binding);

    virtual bool preVisit(CPlusPlus::Symbol *symbol);

    virtual void postVisit(CPlusPlus::Symbol *);

  private: // Members
    QStack<QStandardItem*>                 m_items; // For building the tree model.
    CPlusPlus::CppModel::NamePrettyPrinter m_namePrinter;
};

#endif // SYMBOL_TREEMODEL_H
