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
