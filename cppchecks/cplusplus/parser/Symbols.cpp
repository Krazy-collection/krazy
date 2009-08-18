/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/
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

#include "Symbols.h"
#include "Names.h"
#include "TypeVisitor.h"
#include "SymbolVisitor.h"
#include "Scope.h"
#include <cstdlib>

CPLUSPLUS_BEGIN_NAMESPACE

UsingNamespaceDirective::UsingNamespaceDirective(TranslationUnit *translationUnit,
                                                 unsigned sourceLocation, Name *name)
    : Symbol(translationUnit, sourceLocation, name)
{ }

UsingNamespaceDirective::~UsingNamespaceDirective()
{ }

FullySpecifiedType UsingNamespaceDirective::type() const
{ return FullySpecifiedType(); }

void UsingNamespaceDirective::visitSymbol0(SymbolVisitor *visitor)
{ visitor->visit(this); }

UsingDeclaration::UsingDeclaration(TranslationUnit *translationUnit,
                                   unsigned sourceLocation, Name *name)
    : Symbol(translationUnit, sourceLocation, name)
{ }

UsingDeclaration::~UsingDeclaration()
{ }

FullySpecifiedType UsingDeclaration::type() const
{ return FullySpecifiedType(); }

void UsingDeclaration::visitSymbol0(SymbolVisitor *visitor)
{ visitor->visit(this); }

Declaration::Declaration(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name)
    : Symbol(translationUnit, sourceLocation, name),
      _templateParameters(0)
{ }

Declaration::~Declaration()
{ delete _templateParameters; }

unsigned Declaration::templateParameterCount() const
{
    if (! _templateParameters)
        return 0;
    return _templateParameters->symbolCount();
}

Symbol *Declaration::templateParameterAt(unsigned index) const
{ return _templateParameters->symbolAt(index); }

Scope *Declaration::templateParameters() const
{ return _templateParameters; }

void Declaration::setTemplateParameters(Scope *templateParameters)
{ _templateParameters = templateParameters; }

void Declaration::setType(FullySpecifiedType type)
{ _type = type; }

FullySpecifiedType Declaration::type() const
{ return _type; }

void Declaration::visitSymbol0(SymbolVisitor *visitor)
{ visitor->visit(this); }

Argument::Argument(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name)
    : Symbol(translationUnit, sourceLocation, name),
      _initializer(false)
{ }

Argument::~Argument()
{ }

bool Argument::hasInitializer() const
{ return _initializer; }

void Argument::setInitializer(bool hasInitializer)
{ _initializer = hasInitializer; }

void Argument::setType(FullySpecifiedType type)
{ _type = type; }

FullySpecifiedType Argument::type() const
{ return _type; }

void Argument::visitSymbol0(SymbolVisitor *visitor)
{ visitor->visit(this); }

Function::Function(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name)
    : ScopedSymbol(translationUnit, sourceLocation, name),
     _templateParameters(0),
     _flags(0)
{ _arguments = new Scope(this); }

Function::~Function()
{
    delete _templateParameters;
    delete _arguments;
}

bool Function::isNormal() const
{ return f._methodKey == NormalMethod; }

bool Function::isSignal() const
{ return f._methodKey == SignalMethod; }

bool Function::isSlot() const
{ return f._methodKey == SlotMethod; }

int Function::methodKey() const
{ return f._methodKey; }

void Function::setMethodKey(int key)
{ f._methodKey = key; }

unsigned Function::templateParameterCount() const
{
    if (! _templateParameters)
        return 0;
    return _templateParameters->symbolCount();
}

Symbol *Function::templateParameterAt(unsigned index) const
{ return _templateParameters->symbolAt(index); }

Scope *Function::templateParameters() const
{ return _templateParameters; }

void Function::setTemplateParameters(Scope *templateParameters)
{ _templateParameters = templateParameters; }

bool Function::isEqualTo(const Type *other) const
{
    const Function *o = other->asFunctionType();
    if (! o)
        return false;
    else if (isConst() != o->isConst())
        return false;
    else if (isVolatile() != o->isVolatile())
        return false;

    Name *l = identity();
    Name *r = o->identity();
    if (l == r || (l && l->isEqualTo(r))) {
        if (_arguments->symbolCount() != o->_arguments->symbolCount())
            return false;
        else if (! _returnType.isEqualTo(o->_returnType))
            return false;
        for (unsigned i = 0; i < _arguments->symbolCount(); ++i) {
            Symbol *l = _arguments->symbolAt(i);
            Symbol *r = o->_arguments->symbolAt(i);
            if (! l->type().isEqualTo(r->type()))
                return false;
        }
        return true;
    }
    return false;
}

void Function::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

FullySpecifiedType Function::type() const
{ return FullySpecifiedType(const_cast<Function *>(this)); }

FullySpecifiedType Function::returnType() const
{ return _returnType; }

void Function::setReturnType(FullySpecifiedType returnType)
{ _returnType = returnType; }

bool Function::hasReturnType() const
{
    const FullySpecifiedType ty = returnType();
    return ty.isValid() || ty.isSigned() || ty.isUnsigned();
}

unsigned Function::argumentCount() const
{
    if (! _arguments)
        return 0;

    return _arguments->symbolCount();
}

Symbol *Function::argumentAt(unsigned index) const
{ return _arguments->symbolAt(index); }

Scope *Function::arguments() const
{ return _arguments; }

bool Function::hasArguments() const
{
    return ! (argumentCount() == 0 ||
              (argumentCount() == 1 && argumentAt(0)->type()->isVoidType()));
}

bool Function::isVariadic() const
{ return f._isVariadic; }

void Function::setVariadic(bool isVariadic)
{ f._isVariadic = isVariadic; }

bool Function::isConst() const
{ return f._isConst; }

void Function::setConst(bool isConst)
{ f._isConst = isConst; }

bool Function::isVolatile() const
{ return f._isVolatile; }

void Function::setVolatile(bool isVolatile)
{ f._isVolatile = isVolatile; }

bool Function::isPureVirtual() const
{ return f._isPureVirtual; }

void Function::setPureVirtual(bool isPureVirtual)
{ f._isPureVirtual = isPureVirtual; }

bool Function::isAmbiguous() const
{ return f._isAmbiguous; }

void Function::setAmbiguous(bool isAmbiguous)
{ f._isAmbiguous = isAmbiguous; }

void Function::visitSymbol0(SymbolVisitor *visitor)
{
    if (visitor->visit(this)) {
        for (unsigned i = 0; i < _arguments->symbolCount(); ++i) {
            visitSymbol(_arguments->symbolAt(i), visitor);
        }
        for (unsigned i = 0; i < memberCount(); ++i) {
            visitSymbol(memberAt(i), visitor);
        }
    }
}

ScopedSymbol::ScopedSymbol(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name)
    : Symbol(translationUnit, sourceLocation, name)
{ _members = new Scope(this); }

ScopedSymbol::~ScopedSymbol()
{ delete _members; }

unsigned ScopedSymbol::memberCount() const
{
    if (! _members)
        return 0;
    return _members->symbolCount();
}

Symbol *ScopedSymbol::memberAt(unsigned index) const
{
    if (! _members)
        return 0;
    return _members->symbolAt(index);
}

Scope *ScopedSymbol::members() const
{ return _members; }

void ScopedSymbol::addMember(Symbol *member)
{ _members->enterSymbol(member); }

Block::Block(TranslationUnit *translationUnit, unsigned sourceLocation)
    : ScopedSymbol(translationUnit, sourceLocation, /*name = */ 0)
{ }

Block::~Block()
{ }

FullySpecifiedType Block::type() const
{ return FullySpecifiedType(); }

void Block::visitSymbol0(SymbolVisitor *visitor)
{
    if (visitor->visit(this)) {
        for (unsigned i = 0; i < memberCount(); ++i) {
            visitSymbol(memberAt(i), visitor);
        }
    }
}

Enum::Enum(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name)
    : ScopedSymbol(translationUnit, sourceLocation, name)
{ }

Enum::~Enum()
{ }

FullySpecifiedType Enum::type() const
{ return FullySpecifiedType(const_cast<Enum *>(this)); }

bool Enum::isEqualTo(const Type *other) const
{
    const Enum *o = other->asEnumType();
    if (! o)
        return false;
    Name *l = identity();
    Name *r = o->identity();
    if (l == r)
        return true;
    else if (! l)
        return false;
    return l->isEqualTo(r);
}

void Enum::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

void Enum::visitSymbol0(SymbolVisitor *visitor)
{
    if (visitor->visit(this)) {
        for (unsigned i = 0; i < memberCount(); ++i) {
            visitSymbol(memberAt(i), visitor);
        }
    }
}

Namespace::Namespace(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name)
    : ScopedSymbol(translationUnit, sourceLocation, name)
{ }

Namespace::~Namespace()
{ }

bool Namespace::isEqualTo(const Type *other) const
{
    const Namespace *o = other->asNamespaceType();
    if (! o)
        return false;
    Name *l = identity();
    Name *r = o->identity();
    if (l == r || (l && l->isEqualTo(r)))
        return true;
    return false;
}

void Namespace::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

void Namespace::visitSymbol0(SymbolVisitor *visitor)
{
    if (visitor->visit(this)) {
        for (unsigned i = 0; i < memberCount(); ++i) {
            visitSymbol(memberAt(i), visitor);
        }
    }
}

FullySpecifiedType Namespace::type() const
{ return FullySpecifiedType(const_cast<Namespace *>(this)); }

BaseClass::BaseClass(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name)
    : Symbol(translationUnit, sourceLocation, name),
      _isVirtual(false)
{ }

BaseClass::~BaseClass()
{ }

FullySpecifiedType BaseClass::type() const
{ return FullySpecifiedType(); }

bool BaseClass::isVirtual() const
{ return _isVirtual; }

void BaseClass::setVirtual(bool isVirtual)
{ _isVirtual = isVirtual; }

void BaseClass::visitSymbol0(SymbolVisitor *visitor)
{ visitor->visit(this); }

ForwardClassDeclaration::ForwardClassDeclaration(TranslationUnit *translationUnit,
                                                 unsigned sourceLocation, Name *name)
    : Symbol(translationUnit, sourceLocation, name),
      _templateParameters(0)
{ }

ForwardClassDeclaration::~ForwardClassDeclaration()
{ delete _templateParameters; }

unsigned ForwardClassDeclaration::templateParameterCount() const
{
    if (! _templateParameters)
        return 0;
    return _templateParameters->symbolCount();
}

Symbol *ForwardClassDeclaration::templateParameterAt(unsigned index) const
{ return _templateParameters->symbolAt(index); }

Scope *ForwardClassDeclaration::templateParameters() const
{ return _templateParameters; }

void ForwardClassDeclaration::setTemplateParameters(Scope *templateParameters)
{ _templateParameters = templateParameters; }

FullySpecifiedType ForwardClassDeclaration::type() const
{ return FullySpecifiedType(const_cast<ForwardClassDeclaration *>(this)); }

bool ForwardClassDeclaration::isEqualTo(const Type *other) const
{
    if (const ForwardClassDeclaration *otherClassFwdTy = other->asForwardClassDeclarationType()) {
        if (name() == otherClassFwdTy->name())
            return true;
        else if (name() && otherClassFwdTy->name())
            return name()->isEqualTo(otherClassFwdTy->name());

        return false;
    }
    return false;
}

void ForwardClassDeclaration::visitSymbol0(SymbolVisitor *visitor)
{ visitor->visit(this); }

void ForwardClassDeclaration::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

Class::Class(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name)
    : ScopedSymbol(translationUnit, sourceLocation, name),
      _key(ClassKey),
      _templateParameters(0)
{ }

Class::~Class()
{ delete _templateParameters; }

bool Class::isClass() const
{ return _key == ClassKey; }

bool Class::isStruct() const
{ return _key == StructKey; }

bool Class::isUnion() const
{ return _key == UnionKey; }

Class::Key Class::classKey() const
{ return _key; }

void Class::setClassKey(Key key)
{ _key = key; }

unsigned Class::templateParameterCount() const
{
    if (! _templateParameters)
        return 0;
    return _templateParameters->symbolCount();
}

Symbol *Class::templateParameterAt(unsigned index) const
{ return _templateParameters->symbolAt(index); }

Scope *Class::templateParameters() const
{ return _templateParameters; }

void Class::setTemplateParameters(Scope *templateParameters)
{ _templateParameters = templateParameters; }

void Class::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

unsigned Class::baseClassCount() const
{ return _baseClasses.count(); }

BaseClass *Class::baseClassAt(unsigned index) const
{ return _baseClasses.at(index); }

void Class::addBaseClass(BaseClass *baseClass)
{ _baseClasses.push_back(baseClass); }

FullySpecifiedType Class::type() const
{ return FullySpecifiedType(const_cast<Class *>(this)); }

bool Class::isEqualTo(const Type *other) const
{
    const Class *o = other->asClassType();
    if (! o)
        return false;
    Name *l = identity();
    Name *r = o->identity();
    if (l == r || (l && l->isEqualTo(r)))
        return true;
    else
        return false;
}

void Class::visitSymbol0(SymbolVisitor *visitor)
{
    if (visitor->visit(this)) {
        for (unsigned i = 0; i < _baseClasses.size(); ++i) {
            visitSymbol(_baseClasses.at(i), visitor);
        }
        for (unsigned i = 0; i < memberCount(); ++i) {
            visitSymbol(memberAt(i), visitor);
        }
    }
}

ObjCClass::ObjCClass(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name):
        ScopedSymbol(translationUnit, sourceLocation, name),
        _categoryName(0)
{
}

ObjCClass::~ObjCClass()
{}

FullySpecifiedType ObjCClass::type() const
{ return FullySpecifiedType(const_cast<ObjCClass *>(this)); }

bool ObjCClass::isEqualTo(const Type *other) const
{
    const ObjCClass *o = other->asObjCClassType();
    if (!o)
        return false;

    Name *l = identity();
    Name *r = o->identity();
    if (l == r || (l && l->isEqualTo(r)))
        return true;
    else
        return false;
}

void ObjCClass::visitSymbol0(SymbolVisitor *visitor)
{
    if (visitor->visit(this)) {
        for (unsigned i = 0; i < _baseClasses.size(); ++i)
            visitSymbol(_baseClasses.at(i), visitor);
        for (unsigned i = 0; i < _protocols.size(); ++i)
            visitSymbol(_protocols.at(i), visitor);
    }
}

void ObjCClass::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

ObjCProtocol::ObjCProtocol(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name):
        ScopedSymbol(translationUnit, sourceLocation, name)
{
}

ObjCProtocol::~ObjCProtocol()
{}

FullySpecifiedType ObjCProtocol::type() const
{ return FullySpecifiedType(const_cast<ObjCProtocol *>(this)); }

bool ObjCProtocol::isEqualTo(const Type *other) const
{
    const ObjCProtocol *o = other->asObjCProtocolType();
    if (!o)
        return false;

    Name *l = identity();
    Name *r = o->identity();
    if (l == r || (l && l->isEqualTo(r)))
        return true;
    else
        return false;
}

void ObjCProtocol::visitSymbol0(SymbolVisitor *visitor)
{
    if (visitor->visit(this)) {
        for (unsigned i = 0; i < _protocols.size(); ++i)
            visitSymbol(_protocols.at(i), visitor);
    }
}

void ObjCProtocol::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

ObjCForwardClassDeclaration::ObjCForwardClassDeclaration(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name):
        Symbol(translationUnit, sourceLocation, name)
{
}

ObjCForwardClassDeclaration::~ObjCForwardClassDeclaration()
{}

FullySpecifiedType ObjCForwardClassDeclaration::type() const
{ return FullySpecifiedType(); }

void ObjCForwardClassDeclaration::visitSymbol0(SymbolVisitor *visitor)
{ visitor->visit(this); }

ObjCForwardProtocolDeclaration::ObjCForwardProtocolDeclaration(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name):
        Symbol(translationUnit, sourceLocation, name)
{
}

ObjCForwardProtocolDeclaration::~ObjCForwardProtocolDeclaration()
{}

FullySpecifiedType ObjCForwardProtocolDeclaration::type() const
{ return FullySpecifiedType(); }

void ObjCForwardProtocolDeclaration::visitSymbol0(SymbolVisitor *visitor)
{ visitor->visit(this); }

ObjCMethod::ObjCMethod(TranslationUnit *translationUnit, unsigned sourceLocation, Name *name)
    : ScopedSymbol(translationUnit, sourceLocation, name),
     _flags(0)
{ _arguments = new Scope(this); }

ObjCMethod::~ObjCMethod()
{
    delete _arguments;
}

bool ObjCMethod::isEqualTo(const Type *other) const
{
    const ObjCMethod *o = other->asObjCMethodType();
    if (! o)
        return false;

    Name *l = identity();
    Name *r = o->identity();
    if (l == r || (l && l->isEqualTo(r))) {
        if (_arguments->symbolCount() != o->_arguments->symbolCount())
            return false;
        else if (! _returnType.isEqualTo(o->_returnType))
            return false;
        for (unsigned i = 0; i < _arguments->symbolCount(); ++i) {
            Symbol *l = _arguments->symbolAt(i);
            Symbol *r = o->_arguments->symbolAt(i);
            if (! l->type().isEqualTo(r->type()))
                return false;
        }
        return true;
    }
    return false;
}

void ObjCMethod::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

FullySpecifiedType ObjCMethod::type() const
{ return FullySpecifiedType(const_cast<ObjCMethod *>(this)); }

FullySpecifiedType ObjCMethod::returnType() const
{ return _returnType; }

void ObjCMethod::setReturnType(FullySpecifiedType returnType)
{ _returnType = returnType; }

bool ObjCMethod::hasReturnType() const
{
    const FullySpecifiedType ty = returnType();
    return ty.isValid() || ty.isSigned() || ty.isUnsigned();
}

unsigned ObjCMethod::argumentCount() const
{
    if (! _arguments)
        return 0;

    return _arguments->symbolCount();
}

Symbol *ObjCMethod::argumentAt(unsigned index) const
{ return _arguments->symbolAt(index); }

Scope *ObjCMethod::arguments() const
{ return _arguments; }

bool ObjCMethod::hasArguments() const
{
    return ! (argumentCount() == 0 ||
              (argumentCount() == 1 && argumentAt(0)->type()->isVoidType()));
}

bool ObjCMethod::isVariadic() const
{ return f._isVariadic; }

void ObjCMethod::setVariadic(bool isVariadic)
{ f._isVariadic = isVariadic; }

void ObjCMethod::visitSymbol0(SymbolVisitor *visitor)
{
    if (visitor->visit(this)) {
        for (unsigned i = 0; i < _arguments->symbolCount(); ++i) {
            visitSymbol(_arguments->symbolAt(i), visitor);
        }
        for (unsigned i = 0; i < memberCount(); ++i) {
            visitSymbol(memberAt(i), visitor);
        }
    }
}

CPLUSPLUS_END_NAMESPACE
