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
#include "diagnosticmessage.h"

using namespace CPlusPlus;
using namespace CppModel;

DiagnosticMessage::DiagnosticMessage(Level level
                                   , QString const &fileName
                                   , int line
                                   , int column
                                   , QString const &text)
  : m_column(column)
  , m_fileName(fileName)
  , m_level(level)
  , m_line(line)
  , m_text(text)
{}

unsigned DiagnosticMessage::column() const
{
  return m_column;
}

QString DiagnosticMessage::fileName() const
{
  return m_fileName;
}

bool DiagnosticMessage::isError() const
{
  return m_level == Error;
}

bool DiagnosticMessage::isFatal() const
{
  return m_level == Fatal;
}

bool DiagnosticMessage::isWarning() const
{
  return m_level == Warning;
}

DiagnosticMessage::Level DiagnosticMessage::level() const
{
  return m_level;
}

unsigned DiagnosticMessage::line() const
{
  return m_line;
}

QString DiagnosticMessage::text() const
{
  return m_text;
}
