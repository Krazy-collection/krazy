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
#ifndef CPLUSPLUS_CPPMODEL_CHARBLOCK_H
#define CPLUSPLUS_CPPMODEL_CHARBLOCK_H

namespace CPlusPlus {

namespace CppModel {

  /**
   * Block represents a block of characters in a Document.
   */
  class CharBlock
  {
    public:
      CharBlock(unsigned begin = 0, unsigned end = 0);

      unsigned begin() const;

      bool contains(unsigned pos) const;

      unsigned end() const;

    private:
      unsigned m_begin;
      unsigned m_end;
  };

  inline CharBlock::CharBlock(unsigned begin, unsigned end)
    : m_begin(begin), m_end(end)
  {}

  inline unsigned CharBlock::begin() const
  {
    return m_begin;
  }

  inline bool CharBlock::contains(unsigned pos) const
  {
    return pos >= m_begin && pos < m_end;
  }

  inline unsigned CharBlock::end() const
  {
    return m_end;
  }

} // namespace CppModel

} // namespace CPlusPlus

#endif // CPLUSPLUS_CPPMODEL_CHARBLOCK_H
