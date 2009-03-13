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
#ifndef CPLUSPLUS_CPPMODEL_H
#define CPLUSPLUS_CPPMODEL_H

#include "charblock.h"

#include <preprocessor/Macro.h>
#include <QtCore/QVector>

namespace CPlusPlus {

namespace CppModel {

  /**
   * Internal class to define places in a Document where a macro is used. It
   * contains a reference to that macro which is used and which block in the
   * Document actually is taken by the usage.
   */
  class MacroUse : public CharBlock
  {
    Macro m_macro;
    QVector<CharBlock> m_arguments;

    public:
      MacroUse(Macro const &macro, unsigned begin = 0, unsigned end = 0);

      Macro const &macro() const;

      bool isFunctionLike() const;

      QVector<CharBlock> arguments() const;

      void setArguments(QVector<CharBlock> const &arguments);

      void addArgument(CharBlock const &block);
  };

  inline MacroUse::MacroUse(Macro const &macro, unsigned begin, unsigned end)
    : CharBlock(begin, end)
    , m_macro(macro)
  {}

} // namespace CppModel

} // namespace CPlusPlus

#endif // CPLUSPLUS_CPPMODEL_H
