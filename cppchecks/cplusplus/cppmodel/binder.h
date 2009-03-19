/*
   This file or code within this file was originally part of Qt Creator.

   Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).

   GNU Lesser General Public License Usage

   This file may be used under the terms of the GNU Lesser
   General Public License version 2.1 as published by the Free Software
   Foundation and appearing in the file LICENSE.LGPL included in the
   packaging of this file.  Please review the following information to
   ensure the GNU Lesser General Public License version 2.1 requirements
   will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
*/
#ifndef CPLUSPLUS_CPPMODEL_BINDER_H
#define CPLUSPLUS_CPPMODEL_BINDER_H

#include <parser/CPlusPlusForwardDeclarations.h>
#include <parser/SymbolVisitor.h>

namespace CPlusPlus {

namespace CppModel {
  class Location;
  class NamespaceBinding;

  // We don't export this class because it is only mentioned for internal
  // (in cppmodel) use.
  class Binder : protected SymbolVisitor
  {
    public:
      Binder();
      virtual ~Binder();

      NamespaceBinding *operator()(TranslationUnit *u, Namespace *globals);

    protected:
      NamespaceBinding *bind(Symbol *symbol, NamespaceBinding *binding);
      NamespaceBinding *findOrCreateNamespaceBinding(Namespace *symbol);
      NamespaceBinding *resolveNamespace(Location const &loc, Name *name);
      NamespaceBinding *switchNamespaceBinding(NamespaceBinding *binding);

      using SymbolVisitor::visit;

      virtual bool visit(Namespace *);
      virtual bool visit(UsingNamespaceDirective *);
      virtual bool visit(Class *);
      virtual bool visit(Function *);
      virtual bool visit(Block *);

    private:
      NamespaceBinding *namespaceBinding;
      TranslationUnit *unit;
  };

} // namespace CppModel

} // namespace CPlusPlus

#endif // CPLUSPLUS_CPPMODEL_BINDER_H
