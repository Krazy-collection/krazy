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

#ifndef CPLUSPLUS_CPPMODEL_DIAGNOSTIC_MESSAGE_H
#define CPLUSPLUS_CPPMODEL_DIAGNOSTIC_MESSAGE_H

#include <QtCore/QString>

#include "cppmodel-config.h"

namespace CPlusPlus {

namespace CppModel {

  class CPLUSPLUS_MODEL_EXPORT DiagnosticMessage
  {
    public:
      enum Level {
          Warning,
          Error,
          Fatal
      };

    public:
      DiagnosticMessage(Level level, QString const &fileName,
                        int line, int column,
                        QString const &text);

      unsigned column() const;

      QString fileName() const;

      bool isError() const;

      bool isFatal() const;

      bool isWarning() const;

      Level level() const;

      unsigned line() const;

      QString text() const;

    private:
      unsigned m_column;
      QString  m_fileName;
      Level    m_level;
      unsigned m_line;
      QString  m_text;
  };

} // namespace CppModel

} // namespace CPlusPlus

#endif // CPLUSPLUS_CPPMODEL_DIAGNOSTIC_MESSAGE_H
