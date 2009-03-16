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
