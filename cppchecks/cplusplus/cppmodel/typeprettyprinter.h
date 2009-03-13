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
