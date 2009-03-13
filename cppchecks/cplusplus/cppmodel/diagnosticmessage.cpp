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
