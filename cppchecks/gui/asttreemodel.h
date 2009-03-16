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
#ifndef ASTTREEMODEL_H
#define ASTTREEMODEL_H

#include <parser/ASTVisitor.h>
#include <parser/CPlusPlusForwardDeclarations.h>

#include <QtCore/QStack>
#include <QtGui/QStandardItemModel>

class ASTTreeModel : public QStandardItemModel, private CPlusPlus::ASTVisitor
{
  public: // Functions
    /**
     * Creates a TreeModel for given TranslationUnit. This class will not take
     * ownership of @param translationUnit and you should make shure that this
     * model is not used after deletion of @param translationUnit.
     */
    ASTTreeModel(CPlusPlus::TranslationUnit *translationUnit);

    ~ASTTreeModel();

  private: // Functions
    virtual void postVisit(CPlusPlus::AST* ast);

    virtual bool preVisit(CPlusPlus::AST *ast);

  private: // Members
    QStack<QStandardItem*> m_items; // For building the tree model.
};

#endif // ASTTREEMODEL_H
