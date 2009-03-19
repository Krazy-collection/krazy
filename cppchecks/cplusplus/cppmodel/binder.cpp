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

#include "binder.h"

#include <parser/Symbols.h>
#include <parser/TranslationUnit.h>

#include "location.h"
#include "namespacebinding.h"

using namespace CPlusPlus;
using namespace CppModel;

Binder::Binder()
    : namespaceBinding(0)
    , unit(0)
{ }

Binder::~Binder()
{ }

NamespaceBinding *Binder::operator()(TranslationUnit *u, Namespace *globals)
{
  TranslationUnit *previousUnit = unit;
  unit = u;
  NamespaceBinding *binding = bind(globals, 0);
  unit = previousUnit;
  return binding;
}

NamespaceBinding *Binder::bind(Symbol *symbol, NamespaceBinding *binding)
{
  NamespaceBinding *previousBinding = switchNamespaceBinding(binding);
  accept(symbol);
  return switchNamespaceBinding(previousBinding);
}

NamespaceBinding *Binder::findOrCreateNamespaceBinding(Namespace *symbol)
{
  if (namespaceBinding)
    return namespaceBinding->findOrCreateNamespaceBinding(symbol);

  namespaceBinding = new NamespaceBinding;
  namespaceBinding->symbols.push_back(symbol);
  return namespaceBinding;
}

NamespaceBinding *Binder::resolveNamespace(const Location &loc, Name *name)
{
  if (! namespaceBinding)
    return 0;

  return namespaceBinding->resolveNamespace(loc, name);
}


NamespaceBinding *Binder::switchNamespaceBinding(NamespaceBinding *binding)
{
  NamespaceBinding *previousBinding = namespaceBinding;
  namespaceBinding = binding;
  return previousBinding;
}

bool Binder::visit(Namespace *symbol)
{
  NamespaceBinding *binding = findOrCreateNamespaceBinding(symbol);

  for (unsigned i = 0; i < symbol->memberCount(); ++i)
  {
    Symbol *member = symbol->memberAt(i);
    bind(member, binding);
  }

  return false;
}

bool Binder::visit(UsingNamespaceDirective *u)
{
    NamespaceBinding *resolved = resolveNamespace(Location(u), u->name());

    if (! resolved)
    {
        unit->error(u->sourceLocation(), "expected namespace-name");
        return false;
    }

    namespaceBinding->usings.push_back(resolved);

    return false;
}

bool Binder::visit(Class *)
{
  return false;
}

bool Binder::visit(Function *)
{
  return false;
}

bool Binder::visit(Block *)
{
  return false;
}
