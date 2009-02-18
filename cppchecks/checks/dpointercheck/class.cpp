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
#include "class.h"

Class::Class()
{}

Class::Class(QString const &name) : m_name(name)
{}

Class::Class(Class const &other)
: m_name(other.m_name),
  m_forwardDeclarations(other.m_forwardDeclarations),
  m_typeDefs(other.m_typeDefs)
{}

void Class::addForwardDeclaration(Scope scope, QString const &type)
{
  m_forwardDeclarations.insert(scope, type);
}

void Class::addTypeDef(Scope scope, TypeDef const &def)
{
  m_typeDefs.insert(scope, def);
}

bool Class::containsForwardDeclaration(QString const&type) const
{
  return m_forwardDeclarations.values().contains(type);
}

QStringList Class::forwardDeclarations() const
{
  return m_forwardDeclarations.values();
}

QString Class::name() const
{
  return m_name;
}

QList<TypeDef> Class::typeDefenitions(Scope scope) const
{
  return m_typeDefs.values(scope);
}

QList<TypeDef> Class::typeDefenitions(QList<Scope> scopes, QString const &type) const
{
  QList<TypeDef> result;

  foreach (Scope const scope, scopes)
  {
    QList<TypeDef> typeDefs = m_typeDefs.values(scope);
    foreach (TypeDef const typeDef, typeDefs)
    {
      if (typeDef.type() == type)
        result.append(typeDef);
    }
  }
  
  return result;
}
