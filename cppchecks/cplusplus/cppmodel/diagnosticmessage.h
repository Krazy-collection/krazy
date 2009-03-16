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
