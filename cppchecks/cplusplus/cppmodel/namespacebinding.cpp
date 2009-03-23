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

#include "namespacebinding.h"

#include <cassert>

#include <parser/Name.h>
#include <parser/Names.h>
#include <parser/Scope.h>
#include <parser/Symbols.h>

#include "location.h"

using namespace CPlusPlus;
using namespace CppModel;

NamespaceBinding::NamespaceBinding(NamespaceBinding *parent)
  : parent(parent)
  , anonymousNamespaceBinding(0)
{
  if (parent)
    parent->children.push_back(this);
}

NamespaceBinding::~NamespaceBinding()
{
  for (unsigned i = 0; i < children.size(); ++i)
  {
    NamespaceBinding *binding = children.at(i);
    delete binding;
  }
}

Name *NamespaceBinding::name() const
{
  if (symbols.size())
  {
    if (Name *name = symbols.at(0)->name())
      return name;
  }

  return 0;
}

Identifier *NamespaceBinding::identifier() const
{
  if (NameId *nameId = name()->asNameId())
    return nameId->identifier();

  return 0;
}

NamespaceBinding *NamespaceBinding::globalNamespaceBinding()
{
  NamespaceBinding *it = this;

  for (; it; it = it->parent)
  {
    if (! it->parent)
      break;
  }

  return it;
}

NamespaceBinding *NamespaceBinding::findNamespaceBinding(Name *name)
{
  if (! name)
    return anonymousNamespaceBinding;

  else if (NameId *nameId = name->asNameId())
    return findNamespaceBindingForNameId(nameId);

  // invalid binding
  return 0;
}

NamespaceBinding *NamespaceBinding::findNamespaceBindingForNameId(NameId *name)
{
  for (unsigned i = 0; i < children.size(); ++i)
  {
    NamespaceBinding *binding = children.at(i);
    Name *bindingName = binding->name();

    if (! bindingName)
      continue;

    if (NameId *bindingNameId = bindingName->asNameId())
    {
      if (name->isEqualTo(bindingNameId))
        return binding;
    }
  }

  return 0;
}

NamespaceBinding *NamespaceBinding::findOrCreateNamespaceBinding(Namespace *symbol)
{
  if (NamespaceBinding *binding = findNamespaceBinding(symbol->name()))
  {
    unsigned index = 0;

    for (; index < binding->symbols.size(); ++index)
    {
      Namespace *ns = binding->symbols.at(index);

      if (ns == symbol)
        break;
    }

    if (index == binding->symbols.size())
      binding->symbols.push_back(symbol);

    return binding;
  }

  NamespaceBinding *binding = new NamespaceBinding(this);
  binding->symbols.push_back(symbol);

  if (! symbol->name())
  {
    assert(! anonymousNamespaceBinding);
    anonymousNamespaceBinding = binding;
  }

  return binding;
}

static void closure(Location const &loc,
                    NamespaceBinding *binding, Name *name,
                    Array<NamespaceBinding *> *bindings)
{
  for (unsigned i = 0; i < bindings->size(); ++i)
  {
    NamespaceBinding *b = bindings->at(i);

    if (b == binding)
      return;
  }

  bindings->push_back(binding);

  assert(name->isNameId());

  Identifier *id = name->asNameId()->identifier();
  bool ignoreUsingDirectives = false;

  for (unsigned i = 0; i < binding->symbols.size(); ++i)
  {
    Namespace *symbol = binding->symbols.at(i);
    Scope *scope = symbol->members();

    for (Symbol *symbol = scope->lookat(id); symbol; symbol = symbol->next())
    {
      if (symbol->name() != name || ! symbol->isNamespace())
        continue;

      const Location l(symbol);

      if (l.fileId() == loc.fileId() && l.sourceLocation() < loc.sourceLocation())
      {
        ignoreUsingDirectives = true;
        break;
      }
    }
  }

  if (ignoreUsingDirectives)
    return;

  for (unsigned i = 0; i < binding->usings.size(); ++i)
  {
    NamespaceBinding *u = binding->usings.at(i);
    closure(loc, u, name, bindings);
  }
}

NamespaceBinding *NamespaceBinding::resolveNamespace(Location const &loc,
                                                     Name *name,
                                                     bool lookAtParent)
{
  if (! name)
    return 0;

  else if (NameId *nameId = name->asNameId())
  {
    Array<NamespaceBinding *> bindings;
    closure(loc, this, nameId, &bindings);

    Array<NamespaceBinding *> results;

    for (unsigned i = 0; i < bindings.size(); ++i)
    {
      NamespaceBinding *binding = bindings.at(i);

      if (NamespaceBinding *b = binding->findNamespaceBinding(nameId))
        results.push_back(b);
    }

    if (results.size() == 1)
      return results.at(0);
    else if (results.size() > 1)
    {
      // ### FIXME: return 0;
      return results.at(0);
    }

    else if (parent && lookAtParent)
      return parent->resolveNamespace(loc, name);

  }
  else if (QualifiedNameId *q = name->asQualifiedNameId())
  {
    if (q->nameCount() == 1)
    {
      assert(q->isGlobal());

      return globalNamespaceBinding()->resolveNamespace(loc, q->nameAt(0));
    }

    NamespaceBinding *current = this;
    if (q->isGlobal())
      current = globalNamespaceBinding();

    current = current->resolveNamespace(loc, q->nameAt(0));
    for (unsigned i = 1; current && i < q->nameCount(); ++i)
      current = current->resolveNamespace(loc, q->nameAt(i), false);

    return current;
  }

  return 0;
}
