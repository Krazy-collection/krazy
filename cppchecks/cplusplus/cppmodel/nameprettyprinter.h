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
