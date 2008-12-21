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
#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <QtCore/QString>

/**
 * Representation of a type defenition in a class.
 * 
 * Definition Types:
 * MyClass x;               // VALUE
 * MyClass const y;         // CONST_VALUE
 * MyClass &x;              // REFERENCE_TO_VALUE
 * MyClass const &y;        // REFERENCE_TO_CONST_VALUE
 * MyClass & const y;       // CONST_REFERENCE_TO_VALUE
 * MyClass const & const y; // CONST_REFERENCE_TO_CONST_VALUE
 * MyClass *x;              // POINTER_TO_VALUE
 * MyClass const *y;        // POINTER_TO_CONST_VALUE
 * MyClass * const y;       // CONST_POINTER_TO_VALUE
 * MyClass const * const y; // CONST_POINTER_TO_CONST_VALUE
 */
class TypeDef
{
  public:
    enum DefType
    {
      VALUE,
      CONST_VALUE,
      REFERENCE_TO_VALUE,
      REFERENCE_TO_CONST_VALUE,
      CONST_REFERENCE_TO_VALUE,
      CONST_REFERENCE_TO_CONST_VALUE,
      POINTER_TO_VALUE,
      POINTER_TO_CONST_VALUE,
      CONST_POINTER_TO_VALUE,
      CONST_POINTER_TO_CONST_VALUE
    };

    TypeDef(QString const &type, DefType defType, QString const &name);

    DefType defenitionType() const;

    int line() const;

    QString name() const;

    QString type() const;

    void setLine(int line);

    void setName(QString const &name);

    void setType(QString const &type);

  private:
    DefType m_defType;
    int     m_line;
    QString m_name;
    QString m_type;
};

#endif
