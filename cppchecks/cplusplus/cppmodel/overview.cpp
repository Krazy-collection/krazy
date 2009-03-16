/*
   This file or code within this file was originally part of Qt Creator.

   Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
   Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
   Copyright (c) 2009 Bertjan Broeksema <b.broeksema@kdemail.net>

   GNU Lesser General Public License Usage

   This file may be used under the terms of the GNU Lesser
   General Public License version 2.1 as published by the Free Software
   Foundation and appearing in the file LICENSE.LGPL included in the
   packaging of this file.  Please review the following information to
   ensure the GNU Lesser General Public License version 2.1 requirements
   will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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
