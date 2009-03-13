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
#include "typeprettyprinter.h"

#include <parser/CoreTypes.h>
#include <parser/FullySpecifiedType.h>
#include <parser/Scope.h>
#include <parser/Symbols.h>

#include "overview.h"

using namespace CPlusPlus;
using namespace CppModel;

/// TypePrettyPrinter :: public functions

TypePrettyPrinter::TypePrettyPrinter(Overview const *overview)
    : m_overview(overview),
      m_name(0),
      m_markArgument(0),
      m_showArgumentNames(false),
      m_showReturnTypes(false),
      m_showFunctionSignatures(true)
{}

TypePrettyPrinter::~TypePrettyPrinter()
{}

Overview const *TypePrettyPrinter::overview() const
{
  return m_overview;
}

void TypePrettyPrinter::setMarkArgument(unsigned position)
{
  m_markArgument = position;
}

void TypePrettyPrinter::setShowArgumentNames(bool showArgumentNames)
{
  m_showArgumentNames = showArgumentNames;
}

void TypePrettyPrinter::setShowFunctionSignatures(bool showFunctionSignatures)
{
  m_showFunctionSignatures = showFunctionSignatures;
}

void TypePrettyPrinter::setShowReturnTypes(bool showReturnTypes)
{
  m_showReturnTypes = showReturnTypes;
}

bool TypePrettyPrinter::showArgumentNames() const
{
  return m_showArgumentNames;
}

bool TypePrettyPrinter::showFunctionSignatures() const
{
  return m_showFunctionSignatures;
}

bool TypePrettyPrinter::showReturnTypes() const
{
  return m_showReturnTypes;
}

QString TypePrettyPrinter::operator()(FullySpecifiedType const &ty)
{
  QString previousName = switchText();
  acceptType(ty);
  return switchText(previousName).trimmed();
}

QString TypePrettyPrinter::operator()(FullySpecifiedType const &type, QString const &name)
{
  QString previousName = switchName(name);
  QString text = operator()(type);
  if (!m_name.isEmpty() && !text.isEmpty())
  {
    QChar ch = text.at(text.size() - 1);
    if (ch.isLetterOrNumber() || ch == QLatin1Char('_'))
      text += QLatin1Char(' ');
    text += m_name;
  }
  (void) switchName(previousName);
  return text;
}

/// TypePrettyPrinter :: protected functions

void TypePrettyPrinter::acceptType(const FullySpecifiedType &ty)
{
  if (ty.isConst())
    m_text += QLatin1String("const ");
  if (ty.isVolatile())
    m_text += QLatin1String("volatile ");
  if (ty.isSigned())
    m_text += QLatin1String("signed ");
  if (ty.isUnsigned())
    m_text += QLatin1String("unsigned ");
  accept(ty.type());
}

void TypePrettyPrinter::applyPtrOperators(bool wantSpace)
{
  for (int i = m_ptrOperators.size() - 1; i != -1; --i)
  {
    Type *op = m_ptrOperators.at(i);

    if (i == 0 && wantSpace)
      m_text += QLatin1Char(' ');

    if (PointerType *ptrTy = op->asPointerType())
    {
      m_text += QLatin1Char('*');
      if (ptrTy->elementType().isConst())
        m_text += " const";
      if (ptrTy->elementType().isVolatile())
        m_text += " volatile";
    }
    else if (op->isReferenceType())
      m_text += QLatin1Char('&');
    else if (PointerToMemberType *memPtrTy = op->asPointerToMemberType())
    {
      m_text += QLatin1Char(' ');
      m_text += m_overview->prettyName(memPtrTy->memberName());
      m_text += QLatin1Char('*');
    }
  }
}

QString TypePrettyPrinter::switchName(QString const &name)
{
  QString const previousName = m_name;
  m_name = name;
  return previousName;
}

QList<Type *> TypePrettyPrinter::switchPtrOperators(QList<Type *> const &ptrOperators)
{
  QList<Type *> previousPtrOperators = m_ptrOperators;
  m_ptrOperators = ptrOperators;
  return previousPtrOperators;
}

QString TypePrettyPrinter::switchText(QString const &name)
{
  QString previousName = m_text;
  m_text = name;
  return previousName;
}

void TypePrettyPrinter::visit(ArrayType *type)
{
  m_text += overview()->prettyType(type->elementType());
  if (!m_ptrOperators.isEmpty())
  {
    m_text += QLatin1Char('(');
    applyPtrOperators(false);
    if (!m_name.isEmpty())
    {
        m_text += m_name;
        m_name.clear();
    }
    m_text += QLatin1Char(')');
  }
  m_text += QLatin1String("[]");
}

void TypePrettyPrinter::visit(Class *type)
{
  m_text += overview()->prettyName(type->name());
}

void TypePrettyPrinter::visit(Enum *type)
{
  m_text += overview()->prettyName(type->name());
}

void TypePrettyPrinter::visit(FloatType *type)
{
  switch (type->kind()) {
    case FloatType::Float:
      m_text += QLatin1String("float");
      break;
    case FloatType::Double:
      m_text += QLatin1String("double");
      break;
    case FloatType::LongDouble:
      m_text += QLatin1String("long double");
      break;
  }

  applyPtrOperators();
}

void TypePrettyPrinter::visit(Function *type)
{
  if (m_showReturnTypes)
    m_text += m_overview->prettyType(type->returnType());

    if (!m_ptrOperators.isEmpty())
    {
      m_text += QLatin1Char('(');
      applyPtrOperators(false);
      if (!m_name.isEmpty())
      {
        m_text += m_name;
        m_name.clear();
      }
      m_text += QLatin1Char(')');
    } 
    else if (!m_name.isEmpty() && m_showFunctionSignatures)
    {
      m_text += QLatin1Char(' '); // ### fixme
      m_text += m_name;
      m_name.clear();
    }

    if (m_showFunctionSignatures)
    {
      Overview argumentText;
      m_text += QLatin1Char('(');
      for (unsigned index = 0; index < type->argumentCount(); ++index)
      {
        if (index != 0)
          m_text += QLatin1String(", ");

        if (Argument *arg = type->argumentAt(index)->asArgument())
        {
          if (index + 1 == m_markArgument)
              m_text += QLatin1String("<b>");
          Name *name = 0;
          if (m_showArgumentNames)
              name = arg->name();
          m_text += argumentText(arg->type(), name);
          if (index + 1 == m_markArgument)
            m_text += QLatin1String("</b>");
        }
      }

      if (type->isVariadic())
          m_text += QLatin1String("...");

      m_text += QLatin1Char(')');

      if (type->isConst())
        m_text += QLatin1String(" const");

      if (type->isVolatile())
        m_text += QLatin1String(" volatile");
    }
}

void TypePrettyPrinter::visit(IntegerType *type)
{
  switch (type->kind())
  {
    case IntegerType::Char:
      m_text += QLatin1String("char");
      break;
    case IntegerType::WideChar:
      m_text += QLatin1String("wchar_t");
      break;
    case IntegerType::Bool:
      m_text += QLatin1String("bool");
      break;
    case IntegerType::Short:
      m_text += QLatin1String("short");
      break;
    case IntegerType::Int:
      m_text += QLatin1String("int");
      break;
    case IntegerType::Long:
      m_text += QLatin1String("long");
      break;
    case IntegerType::LongLong:
      m_text += QLatin1String("long long");
      break;
  }

  applyPtrOperators();
}

void TypePrettyPrinter::visit(Namespace *type)
{
  m_text += overview()->prettyName(type->name());
}

void TypePrettyPrinter::visit(NamedType *type)
{
  m_text += overview()->prettyName(type->name());
  applyPtrOperators();
}

void TypePrettyPrinter::visit(PointerToMemberType *type)
{
  m_ptrOperators.append(type);
  acceptType(type->elementType());
}

void TypePrettyPrinter::visit(PointerType *type)
{
  m_ptrOperators.append(type);
  acceptType(type->elementType());
}

void TypePrettyPrinter::visit(ReferenceType *type)
{
  m_ptrOperators.append(type);
  acceptType(type->elementType());
}

void TypePrettyPrinter::visit(VoidType *)
{
  m_text += QLatin1String("void");
  applyPtrOperators();
}
