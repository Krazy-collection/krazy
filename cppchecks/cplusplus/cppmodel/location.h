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
#ifndef CPLUSPLUS_CPPMODEL_LOCATION_H
#define CPLUSPLUS_CPPMODEL_LOCATION_H

#include <parser/Literals.h>
#include <parser/Symbol.h>

#include "cppmodel-config.h"

namespace CPlusPlus {

namespace CppModel {

  class CPLUSPLUS_MODEL_EXPORT Location
  {
    public:
      Location()
        : m_fileId(0),
          m_sourceLocation(0)
      { }

      Location(Symbol *symbol)
        : m_fileId(symbol->fileId()),
          m_sourceLocation(symbol->sourceLocation())
      { }

      Location(StringLiteral *fileId, unsigned sourceLocation)
        : m_fileId(fileId), m_sourceLocation(sourceLocation)
      { }

      inline bool isValid() const
      { return m_fileId != 0; }

      inline operator bool() const
      { return m_fileId != 0; }

      inline StringLiteral *fileId() const
      { return m_fileId; }

      inline unsigned sourceLocation() const
      { return m_sourceLocation; }

    private:
      StringLiteral *m_fileId;
      unsigned m_sourceLocation;
  };

} // namespace CppModel

} // namespace CPlusPlus

#endif // CPLUSPLUS_CPPMODEL_LOCATION_H
