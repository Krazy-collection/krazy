/*
    Copyright (C) 2008 Bertjan Broeksema <b.broeksema@kdemail.net>

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

#include "typedef.h"

TypeDef::TypeDef(QString const &type, DefType defType, QString const &name)
:
  m_defType(defType),
  m_name(name),
  m_type(type)
{}

TypeDef::DefType TypeDef::defenitionType() const
{
  return m_defType;
}

int TypeDef::line() const
{
  return m_line;
}

QString TypeDef::name() const
{
  return m_name;
}

QString TypeDef::type() const
{
  return m_type;
}

void TypeDef::setLine(int line)
{
  m_line = line;
}

void TypeDef::setName(QString const &name)
{
  m_name = name;
}

void TypeDef::setTemplateArguments(QStringList const &arguments)
{
  m_templateArguments = arguments;
}

void TypeDef::setType(QString const &type)
{
  m_type = type;
}

QStringList TypeDef::templateArguments() const
{
  return m_templateArguments;
}
