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

#include "macrouse.h"

using namespace CPlusPlus;
using namespace CppModel;

Macro const &MacroUse::macro() const
{
  return m_macro;
}

bool MacroUse::isFunctionLike() const
{
  return m_macro.isFunctionLike();
}

QVector<CharBlock> MacroUse::arguments() const
{
  return m_arguments;
}

void MacroUse::setArguments(QVector<CharBlock> const &arguments)
{
  m_arguments = arguments;
}

void MacroUse::addArgument(CharBlock const &block)
{
  m_arguments.append(block); 
}
