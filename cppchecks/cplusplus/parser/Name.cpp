/***************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
**
** Non-Open Source Usage
**
** Licensees may use this file in accordance with the Qt Beta Version
** License Agreement, Agreement version 2.2 provided with the Software or,
** alternatively, in accordance with the terms contained in a written
** agreement between you and Nokia.
**
** GNU General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the packaging
** of this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
**
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt GPL Exception
** version 1.3, included in the file GPL_EXCEPTION.txt in this package.
**
***************************************************************************/
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "Name.h"
#include "Names.h"
#include "NameVisitor.h"

CPLUSPLUS_BEGIN_NAMESPACE

Name::Name()
{ }

Name::~Name()
{ }

bool Name::isNameId() const
{ return dynamic_cast<const NameId *>(this) != 0; }

bool Name::isTemplateNameId() const
{ return dynamic_cast<const TemplateNameId *>(this) != 0; }

bool Name::isDestructorNameId() const
{ return dynamic_cast<const DestructorNameId *>(this) != 0; }

bool Name::isOperatorNameId() const
{ return dynamic_cast<const OperatorNameId *>(this) != 0; }

bool Name::isConversionNameId() const
{ return dynamic_cast<const ConversionNameId *>(this) != 0; }

bool Name::isQualifiedNameId() const
{ return dynamic_cast<const QualifiedNameId *>(this) != 0; }

const NameId *Name::asNameId() const
{ return dynamic_cast<const NameId *>(this); }

const TemplateNameId *Name::asTemplateNameId() const
{ return dynamic_cast<const TemplateNameId *>(this); }

const DestructorNameId *Name::asDestructorNameId() const
{ return dynamic_cast<const DestructorNameId *>(this); }

const OperatorNameId *Name::asOperatorNameId() const
{ return dynamic_cast<const OperatorNameId *>(this); }

const ConversionNameId *Name::asConversionNameId() const
{ return dynamic_cast<const ConversionNameId *>(this); }

const QualifiedNameId *Name::asQualifiedNameId() const
{ return dynamic_cast<const QualifiedNameId *>(this); }

NameId *Name::asNameId()
{ return dynamic_cast<NameId *>(this); }

TemplateNameId *Name::asTemplateNameId()
{ return dynamic_cast<TemplateNameId *>(this); }

DestructorNameId *Name::asDestructorNameId()
{ return dynamic_cast<DestructorNameId *>(this); }

OperatorNameId *Name::asOperatorNameId()
{ return dynamic_cast<OperatorNameId *>(this); }

ConversionNameId *Name::asConversionNameId()
{ return dynamic_cast<ConversionNameId *>(this); }

QualifiedNameId *Name::asQualifiedNameId()
{ return dynamic_cast<QualifiedNameId *>(this); }

void Name::accept(NameVisitor *visitor)
{
    if (visitor->preVisit(this))
        accept0(visitor);
    visitor->postVisit(this);
}

void Name::accept(Name *name, NameVisitor *visitor)
{
    if (! name)
        return;
    name->accept(visitor);
}

CPLUSPLUS_END_NAMESPACE
