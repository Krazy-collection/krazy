/*
   This file or code within this file was originally part of Qt Creator.

   Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
   Copyright (C) 2009 Bertjan Broeksema <b.broeksema@kdemail.net>

   GNU Lesser General Public License Usage

   This file may be used under the terms of the GNU Lesser
   General Public License version 2.1 as published by the Free Software
   Foundation and appearing in the file LICENSE.LGPL included in the
   packaging of this file.  Please review the following information to
   ensure the GNU Lesser General Public License version 2.1 requirements
   will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
*/
#ifndef CPLUSPLUS_CPPMODEL_NAMESPACE_BINDING_H
#define CPLUSPLUS_CPPMODEL_NAMESPACE_BINDING_H

#include <parser/Array.h>
#include <parser/CPlusPlusForwardDeclarations.h>

#include "cppmodel-config.h"

namespace CPlusPlus {

namespace CppModel {

  class Location;

  class CPLUSPLUS_MODEL_EXPORT NamespaceBinding
  {
    public:
      /// Constructs a binding with the given parent.
      NamespaceBinding(NamespaceBinding *parent = 0);

      /// Destroys the binding.
      ~NamespaceBinding();

      /// Returns this binding's name.
      Name *name() const;

      /// Returns this binding's identifier.
      Identifier *identifier() const;

      /// Returns the binding for the global namespace (aka ::).
      NamespaceBinding *globalNamespaceBinding();

      /// Returns the binding for the given namespace symbol.
      NamespaceBinding *findNamespaceBinding(Name *name);

      /// Returns the binding associated with the given symbol.
      NamespaceBinding *findOrCreateNamespaceBinding(Namespace *symbol);

      NamespaceBinding *resolveNamespace(Location const &loc,
                                         Name *name,
                                         bool lookAtParent = true);

      /// Helpers.
      //std::string qualifiedId() const;
      //void dump();

    private:
      NamespaceBinding *findNamespaceBindingForNameId(NameId *name);

    public: // attributes
      /// This binding's parent.
      NamespaceBinding *parent;

      /// Binding for anonymous namespace symbols.
      NamespaceBinding *anonymousNamespaceBinding;

      /// This binding's connections.
      Array<NamespaceBinding *> children;

      /// This binding's list of using namespaces.
      Array<NamespaceBinding *> usings;

      /// This binding's namespace symbols.
      Array<Namespace *> symbols;
  };

} // namespace CppModel

} // namespace CPlusPlus

#endif // CPLUSPLUS_CPPMODEL_NAMESPACE_BINDINING_H
