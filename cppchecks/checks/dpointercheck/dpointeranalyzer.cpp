/*
    Copyright (C) 2008 Bertjan Broeksema <b.broeksema@kdemail.net>

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

#include "dpointeranalyzer.h"

#include <cppmodel/nameprettyprinter.h>
#include <cppmodel/overview.h>

#include <iostream>

#include <parser/Scope.h>
#include <parser/Symbol.h>
#include <parser/Symbols.h>

#include <QtCore/QtDebug>

using namespace std;
using namespace CPlusPlus;
using namespace CppModel;

QList<Result> DPointerAnalyzer::analyze(Scope const &globalScope)
{
	qDebug() << "Starting analyze.";
  m_issues.clear();
  analyzeScope(&globalScope);
  return m_issues;
}

void DPointerAnalyzer::analyzeClass(Class const * const cls)
{
  if (!cls->isClass())
    return; // Only check classes, not structs and unions.
  Overview view;
  NamePrettyPrinter pp(&view);
  cout << "Checking class: " << qPrintable(pp(cls->name())) << endl;

  int privateMemberCount = 0;
  Scope *classMembers = cls->members();
  Scope::iterator lastMember = classMembers->lastSymbol();
  for(Scope::iterator it = classMembers->firstSymbol(); it != lastMember; ++it)
  {
    Symbol const * const s = (*it);

    if (s->isDeclaration())
      cout << "Found declaration: " << qPrintable(pp(s->name())) << endl;

  }
}

void DPointerAnalyzer::analyzeScope(Scope const * const scope)
{
  Scope::iterator lastSymbol = scope->lastSymbol();
  for(Scope::iterator it = scope->firstSymbol(); it != lastSymbol; ++it)
  {
    if ((*it)->isNamespace())
      analyzeScope((*it)->asNamespace()->members());
    else if ((*it)->isClass())
      analyzeClass((*it)->asClass());
  }
}
