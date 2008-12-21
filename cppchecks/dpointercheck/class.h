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
#ifndef CLASS_H
#define CLASS_H

#include <QtCore/QList>
#include <QtCore/QMultiMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "typedef.h"

/**
 * Class that represents (although simplified) a class. Used to keep track of
 * issues in the class that is checked.
 */
class Class
{
  public:
    enum Scope
    {
      PUBLIC,
      PROTECTED,
      PRIVATE
    };

  public:
    Class();

    Class(QString const &name);

    Class(Class const &other);

    void addForwardDeclaration(Scope scope, QString const &type);

    void addTypeDef(Scope scope, TypeDef const &def);

    QStringList forwardDeclarations(Scope scope) const;

    QString name() const;

    /**
     * Return all type defenitions that where defined in the given scope.
     */
    QList<TypeDef> typeDefenitions(Scope scope) const;

    /**
     * Return all type defenitions that where defined in one of the given
     * scopes and have the given type.
     */
    QList<TypeDef> typeDefenitions(QList<Scope> scopes, QString const &type) const;

  private:
    QString                   m_name;
    QMultiMap<Scope, QString> m_forwardDeclarations;
    QMultiMap<Scope, TypeDef> m_typeDefs;
};

#endif // CLASS_H
