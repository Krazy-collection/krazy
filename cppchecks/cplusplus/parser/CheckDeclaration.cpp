/***************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CheckDeclaration.h"
#include "Semantic.h"
#include "AST.h"
#include "TranslationUnit.h"
#include "Scope.h"
#include "Names.h"
#include "CoreTypes.h"
#include "Symbols.h"
#include "Control.h"
#include <cassert>

CPLUSPLUS_BEGIN_NAMESPACE

CheckDeclaration::CheckDeclaration(Semantic *semantic)
    : SemanticCheck(semantic),
      _declaration(0),
      _scope(0),
      _templateParameters(0),
      _checkAnonymousArguments(false)
{ }

CheckDeclaration::~CheckDeclaration()
{ }

void CheckDeclaration::check(DeclarationAST *declaration,
                             Scope *scope, Scope *templateParameters)
{
    Scope *previousScope = switchScope(scope);
    Scope *previousTemplateParameters = switchTemplateParameters(templateParameters);
    DeclarationAST *previousDeclaration = switchDeclaration(declaration);
    accept(declaration);
    (void) switchDeclaration(previousDeclaration);
    (void) switchTemplateParameters(previousTemplateParameters);
    (void) switchScope(previousScope);
}

DeclarationAST *CheckDeclaration::switchDeclaration(DeclarationAST *declaration)
{
    DeclarationAST *previousDeclaration = _declaration;
    _declaration = declaration;
    return previousDeclaration;
}

Scope *CheckDeclaration::switchScope(Scope *scope)
{
    Scope *previousScope = _scope;
    _scope = scope;
    return previousScope;
}

Scope *CheckDeclaration::switchTemplateParameters(Scope *templateParameters)
{
    Scope *previousTemplateParameters = _templateParameters;
    _templateParameters = templateParameters;
    return previousTemplateParameters;
}

void CheckDeclaration::checkFunctionArguments(Function *fun)
{
    if (! _checkAnonymousArguments)
        return;

    if (_scope->isClassScope() && fun->isPublic()) {
        for (unsigned argc = 0; argc < fun->argumentCount(); ++argc) {
            Argument *arg = fun->argumentAt(argc)->asArgument();
            assert(arg != 0);

            if (! arg->name()) {
                translationUnit()->warning(arg->sourceLocation(),
                                           "anonymous argument");
            }
        }
    }
}

bool CheckDeclaration::visit(SimpleDeclarationAST *ast)
{
    FullySpecifiedType ty = semantic()->check(ast->decl_specifier_seq, _scope);
    FullySpecifiedType qualTy = ty.qualifiedType();

    if (_templateParameters && ty) {
        if (Class *klass = ty->asClassType()) {
            klass->setTemplateParameters(_templateParameters);
        }
    }

    if (! ast->declarators && ast->decl_specifier_seq && ! ast->decl_specifier_seq->next) {
        if (ElaboratedTypeSpecifierAST *elab_type_spec = ast->decl_specifier_seq->asElaboratedTypeSpecifier()) {
            Name *name = semantic()->check(elab_type_spec->name, _scope);
            ForwardClassDeclaration *symbol =
                    control()->newForwardClassDeclaration(elab_type_spec->firstToken(),
                                                          name);

            if (_templateParameters) {
                symbol->setTemplateParameters(_templateParameters);
                _templateParameters = 0;
            }

            _scope->enterSymbol(symbol);
            return false;
        }
    }

    List<Declaration *> **decl_it = &ast->symbols;
    for (DeclaratorListAST *it = ast->declarators; it; it = it->next) {
        Name *name = 0;
        FullySpecifiedType declTy = semantic()->check(it->declarator, qualTy,
                                                      _scope, &name);

        Function *fun = 0;
        if (declTy && 0 != (fun = declTy->asFunctionType())) {
            fun->setScope(_scope);
            fun->setName(name);
            fun->setMethodKey(semantic()->currentMethodKey());
            fun->setVisibility(semantic()->currentVisibility());
        } else if (semantic()->currentMethodKey() != Function::NormalMethod) {
            translationUnit()->warning(ast->firstToken(),
                                       "expected a function declaration");
        }

        unsigned location = 0;
        if (it->declarator)
            location = it->declarator->firstToken();
        else
            location = ast->firstToken();

        Declaration *symbol = control()->newDeclaration(location, name);
        symbol->setType(control()->integerType(IntegerType::Int));
        symbol->setType(declTy);

        if (_templateParameters && it == ast->declarators && ty && ! ty->isClassType())
            symbol->setTemplateParameters(_templateParameters);

        symbol->setVisibility(semantic()->currentVisibility());

        if (ty.isFriend())
            symbol->setStorage(Symbol::Friend);
        else if (ty.isRegister())
            symbol->setStorage(Symbol::Register);
        else if (ty.isStatic())
            symbol->setStorage(Symbol::Static);
        else if (ty.isExtern())
            symbol->setStorage(Symbol::Extern);
        else if (ty.isMutable())
            symbol->setStorage(Symbol::Mutable);
        else if (ty.isTypedef())
            symbol->setStorage(Symbol::Typedef);

        *decl_it = new (translationUnit()->memoryPool()) List<Declaration *>();
        (*decl_it)->value = symbol;
        decl_it = &(*decl_it)->next;

        _scope->enterSymbol(symbol);
    }
    return false;
}

bool CheckDeclaration::visit(EmptyDeclarationAST *)
{
    return false;
}

bool CheckDeclaration::visit(AccessDeclarationAST *ast)
{
    int accessSpecifier = tokenKind(ast->access_specifier_token);
    int visibility = semantic()->visibilityForAccessSpecifier(accessSpecifier);
    semantic()->switchVisibility(visibility);
    if (ast->slots_token)
        semantic()->switchMethodKey(Function::SlotMethod);
    else if (accessSpecifier == T_SIGNALS)
        semantic()->switchMethodKey(Function::SignalMethod);
    else
        semantic()->switchMethodKey(Function::NormalMethod);
    return false;
}

bool CheckDeclaration::visit(AsmDefinitionAST *)
{
    return false;
}

bool CheckDeclaration::visit(ExceptionDeclarationAST *)
{
    return false;
}

bool CheckDeclaration::visit(FunctionDefinitionAST *ast)
{
    FullySpecifiedType ty = semantic()->check(ast->decl_specifier_seq, _scope);
    FullySpecifiedType qualTy = ty.qualifiedType();
    Name *name = 0;
    FullySpecifiedType funTy = semantic()->check(ast->declarator, qualTy,
                                                 _scope, &name);
    if (! (funTy && funTy->isFunctionType())) {
        translationUnit()->error(ast->firstToken(),
                                 "expected a function prototype");
        return false;
    }

    Function *fun = funTy->asFunctionType();
    fun->setName(name);
    fun->setTemplateParameters(_templateParameters);
    fun->setVisibility(semantic()->currentVisibility());
    fun->setMethodKey(semantic()->currentMethodKey());

    checkFunctionArguments(fun);

    ast->symbol = fun;
    _scope->enterSymbol(fun);

    if (ast->ctor_initializer) {
        bool looksLikeCtor = false;
        if (ty.isValid() || ! fun->identity())
            looksLikeCtor = false;
        else if (fun->identity()->isNameId() || fun->identity()->isTemplateNameId())
            looksLikeCtor = true;

        if (! looksLikeCtor) {
            translationUnit()->error(ast->ctor_initializer->firstToken(),
                                     "only constructors take base initializers");
        }
    }

    const int previousVisibility = semantic()->switchVisibility(Symbol::Public);
    const int previousMethodKey = semantic()->switchMethodKey(Function::NormalMethod);

    semantic()->check(ast->function_body, fun->members());

    semantic()->switchMethodKey(previousMethodKey);
    semantic()->switchVisibility(previousVisibility);

    if (ast->next && ast->next->asEmptyDeclaration()) {
        translationUnit()->warning(ast->next->firstToken(),
                                   "unnecessary semicolon after function block");
    }

    return false;
}

bool CheckDeclaration::visit(LinkageBodyAST *ast)
{
    for (DeclarationAST *decl = ast->declarations; decl; decl = decl->next) {
       semantic()->check(decl, _scope);
    }
    return false;
}

bool CheckDeclaration::visit(LinkageSpecificationAST *ast)
{
    for (DeclarationAST *decl = ast->declaration; decl; decl = decl->next) {
        semantic()->check(decl, _scope);
    }
    return false;
}

bool CheckDeclaration::visit(NamespaceAST *ast)
{
    Identifier *id = identifier(ast->identifier_token);
    Name *namespaceName = control()->nameId(id);
    Namespace *ns = control()->newNamespace(ast->firstToken(), namespaceName);
    ast->symbol = ns;
    _scope->enterSymbol(ns);
    semantic()->check(ast->linkage_body, ns->members()); // ### we'll do the merge later.

    if (ast->next && ast->next->asEmptyDeclaration()) {
        translationUnit()->warning(ast->next->firstToken(),
                                   "unnecessary semicolon after namespace");
    }

    return false;
}

bool CheckDeclaration::visit(NamespaceAliasDefinitionAST *)
{
    return false;
}

bool CheckDeclaration::visit(ParameterDeclarationAST *ast)
{
    Name *argName = 0;
    FullySpecifiedType ty = semantic()->check(ast->type_specifier, _scope);
    FullySpecifiedType argTy = semantic()->check(ast->declarator, ty.qualifiedType(),
                                                 _scope, &argName);
    FullySpecifiedType exprTy = semantic()->check(ast->expression, _scope);
    Argument *arg = control()->newArgument(ast->firstToken(), argName);
    ast->symbol = arg;
    if (ast->expression)
        arg->setInitializer(true);
    arg->setType(argTy);
    _scope->enterSymbol(arg);
    return false;
}

bool CheckDeclaration::visit(TemplateDeclarationAST *ast)
{
    Scope *previousScope = switchScope(new Scope(_scope->owner()));
    for (DeclarationAST *param = ast->template_parameters; param;
            param = param->next) {
       semantic()->check(param, _scope);
    }

    Scope *templateParameters = switchScope(previousScope);
    semantic()->check(ast->declaration, _scope, templateParameters);
    return false;
}

bool CheckDeclaration::visit(TypenameTypeParameterAST *ast)
{
    Name *name = semantic()->check(ast->name, _scope);
    Argument *arg = control()->newArgument(ast->firstToken(), name); // ### new template type
    ast->symbol = arg;
    _scope->enterSymbol(arg);
    return false;
}

bool CheckDeclaration::visit(TemplateTypeParameterAST *ast)
{
    Name *name = semantic()->check(ast->name, _scope);
    Argument *arg = control()->newArgument(ast->firstToken(), name); // ### new template type
    ast->symbol = arg;
    _scope->enterSymbol(arg);
    return false;
}

bool CheckDeclaration::visit(UsingAST *ast)
{
    Name *name = semantic()->check(ast->name, _scope);
    UsingDeclaration *u = control()->newUsingDeclaration(ast->firstToken(), name);
    ast->symbol = u;
    _scope->enterSymbol(u);
    return false;
}

bool CheckDeclaration::visit(UsingDirectiveAST *ast)
{
    Name *name = semantic()->check(ast->name, _scope);
    UsingNamespaceDirective *u = control()->newUsingNamespaceDirective(ast->firstToken(), name);
    ast->symbol = u;
    _scope->enterSymbol(u);
    return false;
}

CPLUSPLUS_END_NAMESPACE
