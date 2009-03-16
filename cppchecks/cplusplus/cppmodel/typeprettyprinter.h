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
#ifndef TYPEPRETTYPRINTER_H
#define TYPEPRETTYPRINTER_H

#include <parser/TypeVisitor.h>

#include <QtCore/QString>
#include <QtCore/QList>

#include "cppmodel-config.h"

namespace CPlusPlus {

  class FullySpecifiedType;

namespace CppModel {

  class Overview;

  class CPLUSPLUS_MODEL_EXPORT TypePrettyPrinter : protected TypeVisitor
  {
    public:
      TypePrettyPrinter(Overview const *overview);

      virtual ~TypePrettyPrinter();

      Overview const *overview() const;

      void setMarkArgument(unsigned position); // 1-based

      void setShowArgumentNames(bool showArgumentNames);

      void setShowFunctionSignatures(bool showFunctionSignatures);

      void setShowReturnTypes(bool showReturnTypes);

      bool showArgumentNames() const;

      bool showFunctionSignatures() const;

      bool showReturnTypes() const;

      QString operator()(FullySpecifiedType const &type);

      QString operator()(FullySpecifiedType const &type, QString const &name);

    protected: // Functions
      void acceptType(FullySpecifiedType const &ty);

      void applyPtrOperators(bool wantSpace = true);

      QString switchName(QString const &name);

      QList<Type *> switchPtrOperators(QList<Type *> const &ptrOperators);

      QString switchText(QString const &text = QString());

      virtual void visit(ArrayType *type);
      virtual void visit(Class *type);
      virtual void visit(Enum *type);
      virtual void visit(FloatType *type);
      virtual void visit(Function *type);
      virtual void visit(IntegerType *type);
      virtual void visit(Namespace *type);
      virtual void visit(NamedType *type);
      virtual void visit(PointerToMemberType *type);
      virtual void visit(PointerType *type);
      virtual void visit(ReferenceType *type);
      virtual void visit(VoidType *type);

    private: // Members
      Overview const *m_overview;
      QString         m_name;
      QString         m_text;
      QList<Type *>   m_ptrOperators;
      unsigned        m_markArgument;
      bool            m_showArgumentNames: 1;
      bool            m_showReturnTypes: 1;
      bool            m_showFunctionSignatures: 1;
  };

} // namespace CppModel

} // namespace CPlusPlus

#endif // TYPEPRETTYPRINTER_H
