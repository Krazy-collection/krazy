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
#include "nameprettyprinter.h"

#include <parser/Literals.h>
#include <parser/Names.h>
#include <parser/NameVisitor.h>

#include "overview.h"

using namespace CPlusPlus;
using namespace CppModel;

/// Overview :: public functions

NamePrettyPrinter::NamePrettyPrinter(Overview const *overview)
  : m_overview(overview)
{}

NamePrettyPrinter::~NamePrettyPrinter()
{}

Overview const *NamePrettyPrinter::overview() const
{
  return m_overview;
}

QString NamePrettyPrinter::operator()(Name *name)
{
  QString previousName = switchName();
  accept(name);
  return switchName(previousName);
}

/// Overview :: protected functions

QString NamePrettyPrinter::switchName(QString const &name)
{
  QString previousName = m_name;
  m_name = name;
  return previousName;
}

void NamePrettyPrinter::visit(ConversionNameId *name)
{
  m_name += QLatin1String("operator ");
  m_name += overview()->prettyType(name->type());
}

void NamePrettyPrinter::visit(DestructorNameId *name)
{
  Identifier *id = name->identifier();
  m_name += QLatin1Char('~');
  m_name += QString::fromLatin1(id->chars(), id->size());
}

void NamePrettyPrinter::visit(NameId *name)
{
  Identifier *id = name->identifier();
  if (id)
    m_name = QString::fromLatin1(id->chars(), id->size());
  else
    m_name = QLatin1String("anonymous");
}

void NamePrettyPrinter::visit(OperatorNameId *name)
{
  m_name += QLatin1String("operator ");
  switch (name->kind()) { // ### i should probably do this in OperatorNameId
    case OperatorNameId::InvalidOp:
        m_name += QLatin1String("<invalid>");
        break;
    case OperatorNameId::NewOp:
        m_name += QLatin1String("new");
        break;
    case OperatorNameId::DeleteOp:
        m_name += QLatin1String("delete");
        break;
    case OperatorNameId::NewArrayOp:
        m_name += QLatin1String("new[]");
        break;
    case OperatorNameId::DeleteArrayOp:
        m_name += QLatin1String("delete[]");
        break;
    case OperatorNameId::PlusOp:
        m_name += QLatin1String("+");
        break;
    case OperatorNameId::MinusOp:
        m_name += QLatin1String("-");
        break;
    case OperatorNameId::StarOp:
        m_name += QLatin1String("*");
        break;
    case OperatorNameId::SlashOp:
        m_name += QLatin1String("/");
        break;
    case OperatorNameId::PercentOp:
        m_name += QLatin1String("%");
        break;
    case OperatorNameId::CaretOp:
        m_name += QLatin1String("^");
        break;
    case OperatorNameId::AmpOp:
        m_name += QLatin1String("&");
        break;
    case OperatorNameId::PipeOp:
        m_name += QLatin1String("|");
        break;
    case OperatorNameId::TildeOp:
        m_name += QLatin1String("~");
        break;
    case OperatorNameId::ExclaimOp:
        m_name += QLatin1String("!");
        break;
    case OperatorNameId::EqualOp:
        m_name += QLatin1String("=");
        break;
    case OperatorNameId::LessOp:
        m_name += QLatin1String("<");
        break;
    case OperatorNameId::GreaterOp:
        m_name += QLatin1String(">");
        break;
    case OperatorNameId::PlusEqualOp:
        m_name += QLatin1String("+=");
        break;
    case OperatorNameId::MinusEqualOp:
        m_name += QLatin1String("-=");
        break;
    case OperatorNameId::StarEqualOp:
        m_name += QLatin1String("*=");
        break;
    case OperatorNameId::SlashEqualOp:
        m_name += QLatin1String("/=");
        break;
    case OperatorNameId::PercentEqualOp:
        m_name += QLatin1String("%=");
        break;
    case OperatorNameId::CaretEqualOp:
        m_name += QLatin1String("^=");
        break;
    case OperatorNameId::AmpEqualOp:
        m_name += QLatin1String("&=");
        break;
    case OperatorNameId::PipeEqualOp:
        m_name += QLatin1String("|=");
        break;
    case OperatorNameId::LessLessOp:
        m_name += QLatin1String("<<");
        break;
    case OperatorNameId::GreaterGreaterOp:
        m_name += QLatin1String(">>");
        break;
    case OperatorNameId::LessLessEqualOp:
        m_name += QLatin1String("<<=");
        break;
    case OperatorNameId::GreaterGreaterEqualOp:
        m_name += QLatin1String(">>=");
        break;
    case OperatorNameId::EqualEqualOp:
        m_name += QLatin1String("==");
        break;
    case OperatorNameId::ExclaimEqualOp:
        m_name += QLatin1String("!=");
        break;
    case OperatorNameId::LessEqualOp:
        m_name += QLatin1String("<=");
        break;
    case OperatorNameId::GreaterEqualOp:
        m_name += QLatin1String(">=");
        break;
    case OperatorNameId::AmpAmpOp:
        m_name += QLatin1String("&&");
        break;
    case OperatorNameId::PipePipeOp:
        m_name += QLatin1String("||");
        break;
    case OperatorNameId::PlusPlusOp:
        m_name += QLatin1String("++");
        break;
    case OperatorNameId::MinusMinusOp:
        m_name += QLatin1String("--");
        break;
    case OperatorNameId::CommaOp:
        m_name += QLatin1String(",");
        break;
    case OperatorNameId::ArrowStarOp:
        m_name += QLatin1String("->*");
        break;
    case OperatorNameId::ArrowOp:
        m_name += QLatin1String("->");
        break;
    case OperatorNameId::FunctionCallOp:
        m_name += QLatin1String("()");
        break;
    case OperatorNameId::ArrayAccessOp:
        m_name += QLatin1String("[]");
        break;
  } // switch
}

void NamePrettyPrinter::visit(QualifiedNameId *name)
{
  if (name->isGlobal())
    m_name += QLatin1String("::");

  for (unsigned index = 0; index < name->nameCount(); ++index) {
    if (index != 0)
      m_name += QLatin1String("::");
    m_name += operator()(name->nameAt(index));
  }
}

void NamePrettyPrinter::visit(TemplateNameId *name)
{
  Identifier *id = name->identifier();
  if (id)
    m_name = QString::fromLatin1(id->chars(), id->size());
  else
    m_name = QLatin1String("anonymous");
  m_name += QLatin1Char('<');

  for (unsigned index = 0; index < name->templateArgumentCount(); ++index) {
    if (index != 0)
      m_name += QLatin1String(", ");

    FullySpecifiedType argTy = name->templateArgumentAt(index);
    QString arg = overview()->prettyType(argTy);
    if (arg.isEmpty())
      m_name += QString::fromLatin1("_Tp%1").arg(index + 1);
    else
      m_name += arg;
  }
  m_name += QLatin1Char('>');
}
