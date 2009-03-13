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
#ifndef CPLUSPLUS_CPPMODEL_NAMEPRETTYPRINTER_H
#define CPLUSPLUS_CPPMODEL_NAMEPRETTYPRINTER_H

#include <parser/NameVisitor.h>

#include <QtCore/QString>

#include "cppmodel-config.h"

namespace CPlusPlus {

namespace CppModel {

  class Overview;

  class CPLUSPLUS_MODEL_EXPORT NamePrettyPrinter : protected NameVisitor
  {
    public:
      NamePrettyPrinter(Overview const *overview);

      virtual ~NamePrettyPrinter();

      Overview const *overview() const;

      QString operator()(Name *name);

    protected: // Functions
      QString switchName(const QString &name = QString());

      virtual void visit(ConversionNameId *name);

      virtual void visit(DestructorNameId *name);

      virtual void visit(NameId *name);

      virtual void visit(OperatorNameId *name);

      virtual void visit(QualifiedNameId *name);

      virtual void visit(TemplateNameId *name);

    private: // Members
      QString         m_name;
      Overview const *m_overview;
  };

} // namespace CppModel

} // namespace CPlusPlus

#endif // CPLUSPLUS_CPPMODEL_NAMEPRETTYPRINTER_H
