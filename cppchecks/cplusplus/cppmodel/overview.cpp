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
#include "overview.h"

#include <parser/FullySpecifiedType.h>

#include "nameprettyprinter.h"
#include "typeprettyprinter.h"

using namespace CPlusPlus;
using namespace CppModel;

Overview::Overview()
  : m_markArgument(0),
    m_showArgumentNames(false),
    m_showReturnTypes(false),
    m_showFunctionSignatures(true)
{}

Overview::~Overview()
{}

QString Overview::prettyName(Name *name) const
{
  NamePrettyPrinter pp(this);
  return pp(name);
}

QString Overview::prettyType(FullySpecifiedType const &ty, Name *name) const
{
  return prettyType(ty, prettyName(name));
}

QString Overview::prettyType(FullySpecifiedType const &ty, QString const &name) const
{
  TypePrettyPrinter pp(this);
  pp.setMarkArgument(m_markArgument);
  pp.setShowArgumentNames(m_showArgumentNames);
  pp.setShowReturnTypes(m_showReturnTypes);
  pp.setShowFunctionSignatures(m_showFunctionSignatures);
  return pp(ty, name);
}

void Overview::setMarkArgument(unsigned position)
{
  m_markArgument = position;
}

void Overview::setShowArgumentNames(bool showArgumentNames)
{
  m_showArgumentNames = showArgumentNames;
}

void Overview::setShowFunctionSignatures(bool showFunctionSignatures)
{
  m_showFunctionSignatures = showFunctionSignatures;
}

void Overview::setShowReturnTypes(bool showReturnTypes)
{
  m_showReturnTypes = showReturnTypes;
}

bool Overview::showArgumentNames() const
{
  return m_showArgumentNames;
}

bool Overview::showFunctionSignatures() const
{
  return m_showFunctionSignatures;
}

bool Overview::showReturnTypes() const
{
  return m_showReturnTypes;
}

QString Overview::operator()(Name *name) const
{
  return prettyName(name);
}

QString Overview::operator()(FullySpecifiedType const &type, Name *name) const
{
  return prettyType(type, name);
}
