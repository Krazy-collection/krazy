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

#ifndef CPLUSPLUS_CORETYPES_H
#define CPLUSPLUS_CORETYPES_H

#include "CPlusPlusForwardDeclarations.h"
#include "Type.h"
#include "FullySpecifiedType.h"
#include <cstddef>

CPLUSPLUS_BEGIN_HEADER
CPLUSPLUS_BEGIN_NAMESPACE

class CPLUSPLUS_EXPORT VoidType: public Type
{
public:
    virtual bool isEqualTo(const Type *other) const;

protected:
    virtual void accept0(TypeVisitor *visitor);
};

class CPLUSPLUS_EXPORT IntegerType: public Type
{
public:
    enum Kind {
        Char,
        WideChar,
        Bool,
        Short,
        Int,
        Long,
        LongLong
    };

public:
    IntegerType(int kind);
    virtual ~IntegerType();

    int kind() const;

    virtual bool isEqualTo(const Type *other) const;

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    int _kind;
};

class CPLUSPLUS_EXPORT FloatType: public Type
{
public:
    enum Kind {
        Float,
        Double,
        LongDouble
    };

public:
    FloatType(int kind);
    virtual ~FloatType();

    int kind() const;

    virtual bool isEqualTo(const Type *other) const;

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    int _kind;
};

class CPLUSPLUS_EXPORT PointerType: public Type
{
public:
    PointerType(FullySpecifiedType elementType);
    virtual ~PointerType();

    FullySpecifiedType elementType() const;

    virtual bool isEqualTo(const Type *other) const;

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    FullySpecifiedType _elementType;
};

class CPLUSPLUS_EXPORT PointerToMemberType: public Type
{
public:
    PointerToMemberType(Name *memberName, FullySpecifiedType elementType);
    virtual ~PointerToMemberType();

    Name *memberName() const;
    FullySpecifiedType elementType() const;

    virtual bool isEqualTo(const Type *other) const;

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    Name *_memberName;
    FullySpecifiedType _elementType;
};

class CPLUSPLUS_EXPORT ReferenceType: public Type
{
public:
    ReferenceType(FullySpecifiedType elementType);
    virtual ~ReferenceType();

    FullySpecifiedType elementType() const;

    virtual bool isEqualTo(const Type *other) const;

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    FullySpecifiedType _elementType;
};

class CPLUSPLUS_EXPORT ArrayType: public Type
{
public:
    ArrayType(FullySpecifiedType elementType, size_t size);
    virtual ~ArrayType();

    FullySpecifiedType elementType() const;
    size_t size() const;

    virtual bool isEqualTo(const Type *other) const;

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    FullySpecifiedType _elementType;
    size_t _size;
};

class CPLUSPLUS_EXPORT NamedType: public Type
{
public:
    NamedType(Name *name);
    virtual ~NamedType();

    Name *name() const;

    virtual bool isEqualTo(const Type *other) const;

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    Name *_name;
};

CPLUSPLUS_END_NAMESPACE
CPLUSPLUS_END_HEADER

#endif // CPLUSPLUS_CORETYPES_H
