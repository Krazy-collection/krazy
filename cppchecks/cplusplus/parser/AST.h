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

#ifndef CPLUSPLUS_AST_H
#define CPLUSPLUS_AST_H

#include "CPlusPlusForwardDeclarations.h"
#include "ASTfwd.h"
#include "MemoryPool.h"

CPLUSPLUS_BEGIN_HEADER
CPLUSPLUS_BEGIN_NAMESPACE

template <typename _Tp>
class List: public Managed
{
    List(const List &other);
    void operator =(const List &other);

public:
    List()
    { }

    _Tp value;
    List *next;
};

class CPLUSPLUS_EXPORT AST: public Managed
{
    AST(const AST &other);
    void operator =(const AST &other);

public:
    AST();
    virtual ~AST();

    void accept(ASTVisitor *visitor);

    static void accept(AST *ast, ASTVisitor *visitor)
    { if (ast) ast->accept(visitor); }

    virtual unsigned firstToken() const = 0;
    virtual unsigned lastToken() const = 0;

    virtual AccessDeclarationAST *asAccessDeclaration() { return 0; }
    virtual ArrayAccessAST *asArrayAccess() { return 0; }
    virtual ArrayDeclaratorAST *asArrayDeclarator() { return 0; }
    virtual ArrayInitializerAST *asArrayInitializer() { return 0; }
    virtual AsmDefinitionAST *asAsmDefinition() { return 0; }
    virtual AttributeAST *asAttribute() { return 0; }
    virtual AttributeSpecifierAST *asAttributeSpecifier() { return 0; }
    virtual BaseSpecifierAST *asBaseSpecifier() { return 0; }
    virtual QtMethodAST *asQtMethod() { return 0; }
    virtual BinaryExpressionAST *asBinaryExpression() { return 0; }
    virtual BoolLiteralAST *asBoolLiteral() { return 0; }
    virtual BreakStatementAST *asBreakStatement() { return 0; }
    virtual CallAST *asCall() { return 0; }
    virtual CaseStatementAST *asCaseStatement() { return 0; }
    virtual CastExpressionAST *asCastExpression() { return 0; }
    virtual CatchClauseAST *asCatchClause() { return 0; }
    virtual ClassSpecifierAST *asClassSpecifier() { return 0; }
    virtual CompoundLiteralAST *asCompoundLiteral() { return 0; }
    virtual CompoundStatementAST *asCompoundStatement() { return 0; }
    virtual ConditionAST *asCondition() { return 0; }
    virtual ConditionalExpressionAST *asConditionalExpression() { return 0; }
    virtual ContinueStatementAST *asContinueStatement() { return 0; }
    virtual ConversionFunctionIdAST *asConversionFunctionId() { return 0; }
    virtual CoreDeclaratorAST *asCoreDeclarator() { return 0; }
    virtual CppCastExpressionAST *asCppCastExpression() { return 0; }
    virtual CtorInitializerAST *asCtorInitializer() { return 0; }
    virtual DeclarationAST *asDeclaration() { return 0; }
    virtual DeclarationStatementAST *asDeclarationStatement() { return 0; }
    virtual DeclaratorAST *asDeclarator() { return 0; }
    virtual DeclaratorIdAST *asDeclaratorId() { return 0; }
    virtual DeclaratorListAST *asDeclaratorList() { return 0; }
    virtual DeleteExpressionAST *asDeleteExpression() { return 0; }
    virtual DestructorNameAST *asDestructorName() { return 0; }
    virtual DoStatementAST *asDoStatement() { return 0; }
    virtual ElaboratedTypeSpecifierAST *asElaboratedTypeSpecifier() { return 0; }
    virtual EmptyDeclarationAST *asEmptyDeclaration() { return 0; }
    virtual EnumSpecifierAST *asEnumSpecifier() { return 0; }
    virtual EnumeratorAST *asEnumerator() { return 0; }
    virtual ExceptionDeclarationAST *asExceptionDeclaration() { return 0; }
    virtual ExceptionSpecificationAST *asExceptionSpecification() { return 0; }
    virtual ExpressionAST *asExpression() { return 0; }
    virtual ExpressionListAST *asExpressionList() { return 0; }
    virtual ExpressionOrDeclarationStatementAST *asExpressionOrDeclarationStatement() { return 0; }
    virtual ExpressionStatementAST *asExpressionStatement() { return 0; }
    virtual ForeachStatementAST *asForeachStatement() { return 0; }
    virtual ForStatementAST *asForStatement() { return 0; }
    virtual FunctionDeclaratorAST *asFunctionDeclarator() { return 0; }
    virtual FunctionDefinitionAST *asFunctionDefinition() { return 0; }
    virtual GotoStatementAST *asGotoStatement() { return 0; }
    virtual IfStatementAST *asIfStatement() { return 0; }
    virtual LabeledStatementAST *asLabeledStatement() { return 0; }
    virtual LinkageBodyAST *asLinkageBody() { return 0; }
    virtual LinkageSpecificationAST *asLinkageSpecification() { return 0; }
    virtual MemInitializerAST *asMemInitializer() { return 0; }
    virtual MemberAccessAST *asMemberAccess() { return 0; }
    virtual NameAST *asName() { return 0; }
    virtual NamedTypeSpecifierAST *asNamedTypeSpecifier() { return 0; }
    virtual NamespaceAST *asNamespace() { return 0; }
    virtual NamespaceAliasDefinitionAST *asNamespaceAliasDefinition() { return 0; }
    virtual NestedDeclaratorAST *asNestedDeclarator() { return 0; }
    virtual NestedExpressionAST *asNestedExpression() { return 0; }
    virtual NestedNameSpecifierAST *asNestedNameSpecifier() { return 0; }
    virtual NewPlacementAST *asNewPlacement() { return 0; }
    virtual NewArrayDeclaratorAST *asNewArrayDeclarator() { return 0; }
    virtual NewExpressionAST *asNewExpression() { return 0; }
    virtual NewInitializerAST *asNewInitializer() { return 0; }
    virtual NewTypeIdAST *asNewTypeId() { return 0; }
    virtual NumericLiteralAST *asNumericLiteral() { return 0; }
    virtual OperatorAST *asOperator() { return 0; }
    virtual OperatorFunctionIdAST *asOperatorFunctionId() { return 0; }
    virtual ParameterDeclarationAST *asParameterDeclaration() { return 0; }
    virtual ParameterDeclarationClauseAST *asParameterDeclarationClause() { return 0; }
    virtual PointerAST *asPointer() { return 0; }
    virtual PointerToMemberAST *asPointerToMember() { return 0; }
    virtual PostIncrDecrAST *asPostIncrDecr() { return 0; }
    virtual PostfixAST *asPostfix() { return 0; }
    virtual PostfixDeclaratorAST *asPostfixDeclarator() { return 0; }
    virtual PostfixExpressionAST *asPostfixExpression() { return 0; }
    virtual PtrOperatorAST *asPtrOperator() { return 0; }
    virtual QualifiedNameAST *asQualifiedName() { return 0; }
    virtual ReferenceAST *asReference() { return 0; }
    virtual ReturnStatementAST *asReturnStatement() { return 0; }
    virtual SimpleDeclarationAST *asSimpleDeclaration() { return 0; }
    virtual SimpleNameAST *asSimpleName() { return 0; }
    virtual SimpleSpecifierAST *asSimpleSpecifier() { return 0; }
    virtual SizeofExpressionAST *asSizeofExpression() { return 0; }
    virtual SpecifierAST *asSpecifier() { return 0; }
    virtual StatementAST *asStatement() { return 0; }
    virtual StringLiteralAST *asStringLiteral() { return 0; }
    virtual SwitchStatementAST *asSwitchStatement() { return 0; }
    virtual TemplateArgumentListAST *asTemplateArgumentList() { return 0; }
    virtual TemplateDeclarationAST *asTemplateDeclaration() { return 0; }
    virtual TemplateIdAST *asTemplateId() { return 0; }
    virtual TemplateTypeParameterAST *asTemplateTypeParameter() { return 0; }
    virtual ThisExpressionAST *asThisExpression() { return 0; }
    virtual ThrowExpressionAST *asThrowExpression() { return 0; }
    virtual TranslationUnitAST *asTranslationUnit() { return 0; }
    virtual TryBlockStatementAST *asTryBlockStatement() { return 0; }
    virtual TypeConstructorCallAST *asTypeConstructorCall() { return 0; }
    virtual TypeIdAST *asTypeId() { return 0; }
    virtual TypeidExpressionAST *asTypeidExpression() { return 0; }
    virtual TypenameCallExpressionAST *asTypenameCallExpression() { return 0; }
    virtual TypenameTypeParameterAST *asTypenameTypeParameter() { return 0; }
    virtual TypeofSpecifierAST *asTypeofSpecifier() { return 0; }
    virtual UnaryExpressionAST *asUnaryExpression() { return 0; }
    virtual UsingAST *asUsing() { return 0; }
    virtual UsingDirectiveAST *asUsingDirective() { return 0; }
    virtual WhileStatementAST *asWhileStatement() { return 0; }
    virtual IdentifierListAST *asIdentifierList() { return 0; }

    virtual ObjCClassForwardDeclarationAST *asObjCClassForwarDeclaration() { return 0; }
    virtual ObjCClassDeclarationAST *asObjCClassDeclaration() { return 0; }
    virtual ObjCProtocolForwardDeclarationAST *asObjCProtocolForwardDeclaration() { return 0; }
    virtual ObjCProtocolDeclarationAST *asObjCProtocolDeclaration() { return 0; }
    virtual ObjCProtocolRefsAST *asObjCProtocolRefs() { return 0; }
    virtual ObjCMessageArgumentAST *asObjCMessageArgument() { return 0; }
    virtual ObjCMessageArgumentListAST *asObjCMessageArgumentList() { return 0; }
    virtual ObjCMessageExpressionAST *asObjCMessageExpression() { return 0; }
    virtual ObjCProtocolExpressionAST *asObjCProtocolExpression() { return 0; }
    virtual ObjCTypeNameAST *asObjCTypeName() { return 0; }
    virtual ObjCEncodeExpressionAST *asObjCEncodeExpression() { return 0; }
    virtual ObjCSelectorAST *asObjCSelector() { return 0; }
    virtual ObjCSelectorWithoutArgumentsAST *asObjCSelectorWithoutArguments() { return 0; }
    virtual ObjCSelectorArgumentAST *asObjCSelectorArgument() { return 0; }
    virtual ObjCSelectorArgumentListAST *asObjCSelectorArgumentList() { return 0; }
    virtual ObjCSelectorWithArgumentsAST *asObjCSelectorWithArguments() { return 0; }
    virtual ObjCSelectorExpressionAST *asObjCSelectorExpression() { return 0; }
    virtual ObjCInstanceVariablesDeclarationAST *asObjCInstanceVariablesDeclaration() { return 0; }
    virtual ObjCVisibilityDeclarationAST *asObjCVisibilityDeclaration() { return 0; }
    virtual ObjCPropertyAttributeAST *asObjCPropertyAttribute() { return 0; }
    virtual ObjCPropertyAttributeListAST *asObjCPropertyAttributeList() { return 0; }
    virtual ObjCPropertyDeclarationAST *asObjCPropertyDeclaration() { return 0; }
    virtual ObjCMessageArgumentDeclarationAST *asObjCMessageArgumentDeclaration() { return 0; }
    virtual ObjCMessageArgumentDeclarationListAST *asObjCMessageArgumentDeclarationList() { return 0; }
    virtual ObjCMethodPrototypeAST *asObjCMethodPrototype() { return 0; }
    virtual ObjCMethodDeclarationAST *asObjCMethodDeclaration() { return 0; }
    virtual ObjCSynthesizedPropertyAST *asObjCSynthesizedProperty() { return 0; }
    virtual ObjCSynthesizedPropertyListAST *asObjCSynthesizedPropertyList() { return 0; }
    virtual ObjCSynthesizedPropertiesDeclarationAST *asObjCSynthesizedPropertiesDeclaration() { return 0; }
    virtual ObjCDynamicPropertiesDeclarationAST *asObjCDynamicPropertiesDeclaration() { return 0; }
    virtual ObjCFastEnumerationAST *asObjCFastEnumeration() { return 0; }

    virtual AST *clone(MemoryPool *pool) const = 0;

protected:
    virtual void accept0(ASTVisitor *visitor) = 0;
};

class CPLUSPLUS_EXPORT SpecifierAST: public AST
{
public:
    SpecifierAST *next;

public:
    virtual SpecifierAST *asSpecifier()
    { return this; }

    virtual SpecifierAST *clone(MemoryPool *pool) const = 0;
};

class CPLUSPLUS_EXPORT SimpleSpecifierAST: public SpecifierAST
{
public:
    unsigned specifier_token;

public:
    virtual SimpleSpecifierAST *asSimpleSpecifier()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SimpleSpecifierAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT AttributeSpecifierAST: public SpecifierAST
{
public:
    unsigned attribute_token;
    unsigned first_lparen_token;
    unsigned second_lparen_token;
    AttributeAST *attributes;
    unsigned first_rparen_token;
    unsigned second_rparen_token;

public:
    virtual AttributeSpecifierAST *asAttributeSpecifier()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AttributeSpecifierAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT AttributeAST: public AST
{
public:
    unsigned identifier_token;
    unsigned lparen_token;
    unsigned tag_token;
    ExpressionListAST *expression_list;
    unsigned rparen_token;
    AttributeAST *next;
    unsigned comma_token;

public:
    virtual AttributeAST *asAttribute()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AttributeAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TypeofSpecifierAST: public SpecifierAST
{
public:
    unsigned typeof_token;
    unsigned lparen_token;
    ExpressionAST *expression;
    unsigned rparen_token;

public:
    virtual TypeofSpecifierAST *asTypeofSpecifier()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypeofSpecifierAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT StatementAST: public AST
{
public:
    virtual StatementAST *asStatement()
    { return this; }

    virtual StatementAST *clone(MemoryPool *pool) const = 0;
};

class CPLUSPLUS_EXPORT ExpressionAST: public AST
{
public:
    virtual ExpressionAST *asExpression()
    { return this; }

    virtual ExpressionAST *clone(MemoryPool *pool) const = 0;
};

class CPLUSPLUS_EXPORT DeclarationAST: public AST
{
public:
    virtual DeclarationAST *asDeclaration()
    { return this; }

    virtual DeclarationAST *clone(MemoryPool *pool) const = 0;
};

class CPLUSPLUS_EXPORT DeclarationListAST: public AST
{
public:
    DeclarationAST *declaration;
    DeclarationListAST *next;

public:
    virtual DeclarationListAST *asDeclarationList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeclarationListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT CoreDeclaratorAST: public AST
{
public:
    virtual CoreDeclaratorAST *asCoreDeclarator()
    { return this; }

    virtual CoreDeclaratorAST *clone(MemoryPool *pool) const = 0;
};

class CPLUSPLUS_EXPORT PostfixDeclaratorAST: public AST
{
public:
    PostfixDeclaratorAST *next;

public:
    virtual PostfixDeclaratorAST *asPostfixDeclarator()
    { return this; }

    virtual PostfixDeclaratorAST *clone(MemoryPool *pool) const = 0;
};

class CPLUSPLUS_EXPORT DeclaratorAST: public AST
{
public:
    SpecifierAST *attributes;
    PtrOperatorAST *ptr_operators;
    CoreDeclaratorAST *core_declarator;
    PostfixDeclaratorAST *postfix_declarators;
    SpecifierAST *post_attributes;
    unsigned equals_token;
    ExpressionAST *initializer;

public:
    virtual DeclaratorAST *asDeclarator()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeclaratorAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ExpressionListAST: public AST
{
public:
    unsigned comma_token;
    ExpressionAST *expression;
    ExpressionListAST *next;

public:
    virtual ExpressionListAST *asExpressionList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ExpressionListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT SimpleDeclarationAST: public DeclarationAST
{
public:
    unsigned qt_invokable_token;
    SpecifierAST *decl_specifier_seq;
    DeclaratorListAST *declarators;
    unsigned semicolon_token;

public:
    List<Declaration *> *symbols;

public:
    virtual SimpleDeclarationAST *asSimpleDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SimpleDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT EmptyDeclarationAST: public DeclarationAST
{
public:
    unsigned semicolon_token;

public:
    virtual EmptyDeclarationAST *asEmptyDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual EmptyDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT AccessDeclarationAST: public DeclarationAST
{
public:
    unsigned access_specifier_token;
    unsigned slots_token;
    unsigned colon_token;

public:
    virtual AccessDeclarationAST *asAccessDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AccessDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT AsmDefinitionAST: public DeclarationAST
{
public:
    unsigned asm_token;
    unsigned volatile_token;
    unsigned lparen_token;
    // ### string literals
    // ### asm operand list
    unsigned rparen_token;
    unsigned semicolon_token;

public:
    virtual AsmDefinitionAST *asAsmDefinition()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AsmDefinitionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT BaseSpecifierAST: public AST
{
public:
    unsigned comma_token;
    unsigned virtual_token;
    unsigned access_specifier_token;
    NameAST *name;
    BaseSpecifierAST *next;

public: // annotations
    BaseClass *symbol;

public:
    virtual BaseSpecifierAST *asBaseSpecifier()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BaseSpecifierAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT CompoundLiteralAST: public ExpressionAST
{
public:
    unsigned lparen_token;
    ExpressionAST *type_id;
    unsigned rparen_token;
    ExpressionAST *initializer;

public:
    virtual CompoundLiteralAST *asCompoundLiteral()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CompoundLiteralAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT QtMethodAST: public ExpressionAST
{
public:
    unsigned method_token;
    unsigned lparen_token;
    DeclaratorAST *declarator;
    unsigned rparen_token;

public:
    virtual QtMethodAST *asQtMethod()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual QtMethodAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT BinaryExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *left_expression;
    unsigned binary_op_token;
    ExpressionAST *right_expression;

public:
    virtual BinaryExpressionAST *asBinaryExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BinaryExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT CastExpressionAST: public ExpressionAST
{
public:
    unsigned lparen_token;
    ExpressionAST *type_id;
    unsigned rparen_token;
    ExpressionAST *expression;

public:
    virtual CastExpressionAST *asCastExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CastExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ClassSpecifierAST: public SpecifierAST
{
public:
    unsigned classkey_token;
    SpecifierAST *attributes;
    NameAST *name;
    unsigned colon_token;
    BaseSpecifierAST *base_clause;
    unsigned lbrace_token;
    DeclarationListAST *member_specifiers;
    unsigned rbrace_token;

public: // annotations
    Class *symbol;

public:
    virtual ClassSpecifierAST *asClassSpecifier()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ClassSpecifierAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT CaseStatementAST: public StatementAST
{
public:
    unsigned case_token;
    ExpressionAST *expression;
    unsigned colon_token;
    StatementAST *statement;

public:
    virtual CaseStatementAST *asCaseStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CaseStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT StatementListAST: public AST
{
public:
    StatementAST *statement;
    StatementListAST *next;

public:
    virtual StatementListAST *asStatementList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual StatementListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT CompoundStatementAST: public StatementAST
{
public:
    unsigned lbrace_token;
    StatementListAST *statements;
    unsigned rbrace_token;

public: // annotations
    Block *symbol;

public:
    virtual CompoundStatementAST *asCompoundStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CompoundStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ConditionAST: public ExpressionAST
{
public:
    SpecifierAST *type_specifier;
    DeclaratorAST *declarator;

public:
    virtual ConditionAST *asCondition()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ConditionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ConditionalExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *condition;
    unsigned question_token;
    ExpressionAST *left_expression;
    unsigned colon_token;
    ExpressionAST *right_expression;

public:
    virtual ConditionalExpressionAST *asConditionalExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ConditionalExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT CppCastExpressionAST: public ExpressionAST
{
public:
    unsigned cast_token;
    unsigned less_token;
    ExpressionAST *type_id;
    unsigned greater_token;
    unsigned lparen_token;
    ExpressionAST *expression;
    unsigned rparen_token;

public:
    virtual CppCastExpressionAST *asCppCastExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CppCastExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT CtorInitializerAST: public AST
{
public:
    unsigned colon_token;
    MemInitializerAST *member_initializers;

public:
    virtual CtorInitializerAST *asCtorInitializer()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CtorInitializerAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT DeclarationStatementAST: public StatementAST
{
public:
    DeclarationAST *declaration;

public:
    virtual DeclarationStatementAST *asDeclarationStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeclarationStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT DeclaratorIdAST: public CoreDeclaratorAST
{
public:
    NameAST *name;

public:
    virtual DeclaratorIdAST *asDeclaratorId()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeclaratorIdAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NestedDeclaratorAST: public CoreDeclaratorAST
{
public:
    unsigned lparen_token;
    DeclaratorAST *declarator;
    unsigned rparen_token;

public:
    virtual NestedDeclaratorAST *asNestedDeclarator()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NestedDeclaratorAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT FunctionDeclaratorAST: public PostfixDeclaratorAST
{
public:
    unsigned lparen_token;
    ParameterDeclarationClauseAST *parameters;
    unsigned rparen_token;
    SpecifierAST *cv_qualifier_seq;
    ExceptionSpecificationAST *exception_specification;
    ExpressionAST *as_cpp_initializer;

public: // annotations
    Function *symbol;

public:
    virtual FunctionDeclaratorAST *asFunctionDeclarator()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual FunctionDeclaratorAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ArrayDeclaratorAST: public PostfixDeclaratorAST
{
public:
    unsigned lbracket_token;
    ExpressionAST *expression;
    unsigned rbracket_token;

public:
    virtual ArrayDeclaratorAST *asArrayDeclarator()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ArrayDeclaratorAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT DeclaratorListAST: public AST
{
public:
    unsigned comma_token;
    DeclaratorAST *declarator;
    DeclaratorListAST *next;

public:
    virtual DeclaratorListAST *asDeclaratorList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeclaratorListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT DeleteExpressionAST: public ExpressionAST
{
public:
    unsigned scope_token;
    unsigned delete_token;
    unsigned lbracket_token;
    unsigned rbracket_token;
    ExpressionAST *expression;

public:
    virtual DeleteExpressionAST *asDeleteExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeleteExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT DoStatementAST: public StatementAST
{
public:
    unsigned do_token;
    StatementAST *statement;
    unsigned while_token;
    unsigned lparen_token;
    ExpressionAST *expression;
    unsigned rparen_token;
    unsigned semicolon_token;

public:
    virtual DoStatementAST *asDoStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DoStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NamedTypeSpecifierAST: public SpecifierAST
{
public:
    NameAST *name;

public:
    virtual NamedTypeSpecifierAST *asNamedTypeSpecifier()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NamedTypeSpecifierAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ElaboratedTypeSpecifierAST: public SpecifierAST
{
public:
    unsigned classkey_token;
    NameAST *name;

public:
    virtual ElaboratedTypeSpecifierAST *asElaboratedTypeSpecifier()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ElaboratedTypeSpecifierAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT EnumSpecifierAST: public SpecifierAST
{
public:
    unsigned enum_token;
    NameAST *name;
    unsigned lbrace_token;
    EnumeratorAST *enumerators;
    unsigned rbrace_token;

public:
    virtual EnumSpecifierAST *asEnumSpecifier()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual EnumSpecifierAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT EnumeratorAST: public AST
{
public:
    unsigned comma_token;
    unsigned identifier_token;
    unsigned equal_token;
    ExpressionAST *expression;
    EnumeratorAST *next;

public:
    virtual EnumeratorAST *asEnumerator()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual EnumeratorAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ExceptionDeclarationAST: public DeclarationAST
{
public:
    SpecifierAST *type_specifier;
    DeclaratorAST *declarator;
    unsigned dot_dot_dot_token;

public:
    virtual ExceptionDeclarationAST *asExceptionDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ExceptionDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ExceptionSpecificationAST: public AST
{
public:
    unsigned throw_token;
    unsigned lparen_token;
    unsigned dot_dot_dot_token;
    ExpressionListAST *type_ids;
    unsigned rparen_token;

public:
    virtual ExceptionSpecificationAST *asExceptionSpecification()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ExceptionSpecificationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ExpressionOrDeclarationStatementAST: public StatementAST
{
public:
    StatementAST *expression;
    StatementAST *declaration;

public:
    virtual ExpressionOrDeclarationStatementAST *asExpressionOrDeclarationStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ExpressionOrDeclarationStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ExpressionStatementAST: public StatementAST
{
public:
    ExpressionAST *expression;
    unsigned semicolon_token;

public:
    virtual ExpressionStatementAST *asExpressionStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ExpressionStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT FunctionDefinitionAST: public DeclarationAST
{
public:
    unsigned qt_invokable_token;
    SpecifierAST *decl_specifier_seq;
    DeclaratorAST *declarator;
    CtorInitializerAST *ctor_initializer;
    StatementAST *function_body;

public: // annotations
    Function *symbol;

public:
    virtual FunctionDefinitionAST *asFunctionDefinition()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual FunctionDefinitionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ForeachStatementAST: public StatementAST
{
public:
    unsigned foreach_token;
    unsigned lparen_token;
    // declaration
    SpecifierAST *type_specifiers;
    DeclaratorAST *declarator;
    // or an expression
    ExpressionAST *initializer;
    unsigned comma_token;
    ExpressionAST *expression;
    unsigned rparen_token;
    StatementAST *statement;

public: // annotations
    Block *symbol;

public:
    virtual ForeachStatementAST *asForeachStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ForeachStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ForStatementAST: public StatementAST
{
public:
    unsigned for_token;
    unsigned lparen_token;
    StatementAST *initializer;
    ExpressionAST *condition;
    unsigned semicolon_token;
    ExpressionAST *expression;
    unsigned rparen_token;
    StatementAST *statement;

public: // annotations
    Block *symbol;

public:
    virtual ForStatementAST *asForStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ForStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT IfStatementAST: public StatementAST
{
public:
    unsigned if_token;
    unsigned lparen_token;
    ExpressionAST *condition;
    unsigned rparen_token;
    StatementAST *statement;
    unsigned else_token;
    StatementAST *else_statement;

public: // annotations
    Block *symbol;

public:
    virtual IfStatementAST *asIfStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual IfStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ArrayInitializerAST: public ExpressionAST
{
public:
    unsigned lbrace_token;
    ExpressionListAST *expression_list;
    unsigned rbrace_token;

public:
    virtual ArrayInitializerAST *asArrayInitializer()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ArrayInitializerAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT LabeledStatementAST: public StatementAST
{
public:
    unsigned label_token;
    unsigned colon_token;
    StatementAST *statement;

public:
    virtual LabeledStatementAST *asLabeledStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LabeledStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT LinkageBodyAST: public DeclarationAST
{
public:
    unsigned lbrace_token;
    DeclarationListAST *declarations;
    unsigned rbrace_token;

public:
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LinkageBodyAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT LinkageSpecificationAST: public DeclarationAST
{
public:
    unsigned extern_token;
    unsigned extern_type_token;
    DeclarationAST *declaration;

public:
    virtual LinkageSpecificationAST *asLinkageSpecification()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LinkageSpecificationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT MemInitializerAST: public AST
{
public:
    unsigned comma_token;
    NameAST *name;
    unsigned lparen_token;
    ExpressionAST *expression;
    unsigned rparen_token;
    MemInitializerAST *next;

public:
    virtual MemInitializerAST *asMemInitializer()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual MemInitializerAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NameAST: public ExpressionAST
{
public: // annotations
    Name *name;

    virtual NameAST *asName() { return this; }

public:
    virtual NameAST *clone(MemoryPool *pool) const = 0;
};

class CPLUSPLUS_EXPORT NestedNameSpecifierAST: public AST
{
public:
    NameAST *class_or_namespace_name;
    unsigned scope_token;
    NestedNameSpecifierAST *next;

public:
    virtual NestedNameSpecifierAST *asNestedNameSpecifier()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NestedNameSpecifierAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT QualifiedNameAST: public NameAST
{
public:
    unsigned global_scope_token;
    NestedNameSpecifierAST *nested_name_specifier;
    NameAST *unqualified_name;

public:
    virtual QualifiedNameAST *asQualifiedName()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual QualifiedNameAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT OperatorFunctionIdAST: public NameAST
{
public:
    unsigned operator_token;
    OperatorAST *op;

public:
    virtual OperatorFunctionIdAST *asOperatorFunctionId()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual OperatorFunctionIdAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ConversionFunctionIdAST: public NameAST
{
public:
    unsigned operator_token;
    SpecifierAST *type_specifier;
    PtrOperatorAST *ptr_operators;

public:
    virtual ConversionFunctionIdAST *asConversionFunctionId()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ConversionFunctionIdAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT SimpleNameAST: public NameAST
{
public:
    unsigned identifier_token;

public:
    virtual SimpleNameAST *asSimpleName()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SimpleNameAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT DestructorNameAST: public NameAST
{
public:
    unsigned tilde_token;
    unsigned identifier_token;

public:
    virtual DestructorNameAST *asDestructorName()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DestructorNameAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TemplateIdAST: public NameAST
{
public:
    unsigned identifier_token;
    unsigned less_token;
    TemplateArgumentListAST *template_arguments;
    unsigned greater_token;

public:
    virtual TemplateIdAST *asTemplateId()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TemplateIdAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NamespaceAST: public DeclarationAST
{
public:
    unsigned namespace_token;
    unsigned identifier_token;
    SpecifierAST *attributes;
    DeclarationAST *linkage_body;

public: // annotations
    Namespace *symbol;

public:
    virtual NamespaceAST *asNamespace()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NamespaceAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NamespaceAliasDefinitionAST: public DeclarationAST
{
public:
    unsigned namespace_token;
    unsigned namespace_name_token;
    unsigned equal_token;
    NameAST *name;
    unsigned semicolon_token;

public:
    virtual NamespaceAliasDefinitionAST *asNamespaceAliasDefinition()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NamespaceAliasDefinitionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NewPlacementAST: public AST
{
public:
    unsigned lparen_token;
    ExpressionListAST *expression_list;
    unsigned rparen_token;

public:
    virtual NewPlacementAST *asNewPlacement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NewPlacementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NewArrayDeclaratorAST: public AST
{
public:
    unsigned lbracket_token;
    ExpressionAST *expression;
    unsigned rbracket_token;
    NewArrayDeclaratorAST *next;

public:
    virtual NewArrayDeclaratorAST *asNewArrayDeclarator()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NewArrayDeclaratorAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NewExpressionAST: public ExpressionAST
{
public:
    unsigned scope_token;
    unsigned new_token;
    NewPlacementAST *new_placement;

    unsigned lparen_token;
    ExpressionAST *type_id;
    unsigned rparen_token;

    NewTypeIdAST *new_type_id;

    NewInitializerAST *new_initializer;

public:
    virtual NewExpressionAST *asNewExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NewExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NewInitializerAST: public AST
{
public:
    unsigned lparen_token;
    ExpressionAST *expression;
    unsigned rparen_token;

public:
    virtual NewInitializerAST *asNewInitializer()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NewInitializerAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NewTypeIdAST: public AST
{
public:
    SpecifierAST *type_specifier;
    PtrOperatorAST *ptr_operators;
    NewArrayDeclaratorAST *new_array_declarators;

public:
    virtual NewTypeIdAST *asNewTypeId()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NewTypeIdAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT OperatorAST: public AST
{
public:
    unsigned op_token;
    unsigned open_token;
    unsigned close_token;

public:
    virtual OperatorAST *asOperator()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual OperatorAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ParameterDeclarationAST: public DeclarationAST
{
public:
    SpecifierAST *type_specifier;
    DeclaratorAST *declarator;
    unsigned equal_token;
    ExpressionAST *expression;

public: // annotations
    Argument *symbol;

public:
    virtual ParameterDeclarationAST *asParameterDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ParameterDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ParameterDeclarationClauseAST: public AST
{
public:
    DeclarationListAST *parameter_declarations;
    unsigned dot_dot_dot_token;

public:
    virtual ParameterDeclarationClauseAST *asParameterDeclarationClause()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ParameterDeclarationClauseAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT PostfixAST: public AST
{
public:
    PostfixAST *next;

public:
    virtual PostfixAST *asPostfix()
    { return this; }

    virtual PostfixAST *clone(MemoryPool *pool) const = 0;
};

class CPLUSPLUS_EXPORT CallAST: public PostfixAST
{
public:
    unsigned lparen_token;
    ExpressionListAST *expression_list;
    unsigned rparen_token;

public:
    virtual CallAST *asCall()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CallAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ArrayAccessAST: public PostfixAST
{
public:
    unsigned lbracket_token;
    ExpressionAST *expression;
    unsigned rbracket_token;

public:
    virtual ArrayAccessAST *asArrayAccess()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ArrayAccessAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT PostIncrDecrAST: public PostfixAST
{
public:
    unsigned incr_decr_token;

public:
    virtual PostIncrDecrAST *asPostIncrDecr()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual PostIncrDecrAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT MemberAccessAST: public PostfixAST
{
public:
    unsigned access_token;
    unsigned template_token;
    NameAST *member_name;

public:
    virtual MemberAccessAST *asMemberAccess()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual MemberAccessAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TypeidExpressionAST: public ExpressionAST
{
public:
    unsigned typeid_token;
    unsigned lparen_token;
    ExpressionAST *expression;
    unsigned rparen_token;

public:
    virtual TypeidExpressionAST *asTypeidExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypeidExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TypenameCallExpressionAST: public ExpressionAST
{
public:
    unsigned typename_token;
    NameAST *name;
    unsigned lparen_token;
    ExpressionListAST *expression_list;
    unsigned rparen_token;

public:
    virtual TypenameCallExpressionAST *asTypenameCallExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypenameCallExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TypeConstructorCallAST: public ExpressionAST
{
public:
    SpecifierAST *type_specifier;
    unsigned lparen_token;
    ExpressionListAST *expression_list;
    unsigned rparen_token;

public:
    virtual TypeConstructorCallAST *asTypeConstructorCall()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypeConstructorCallAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT PostfixExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *base_expression;
    PostfixAST *postfix_expressions;

public:
    virtual PostfixExpressionAST *asPostfixExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual PostfixExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT PtrOperatorAST: public AST
{
public:
    PtrOperatorAST *next;

public:
    virtual PtrOperatorAST *asPtrOperator()
    { return this; }

    virtual PtrOperatorAST *clone(MemoryPool *pool) const = 0;
};

class CPLUSPLUS_EXPORT PointerToMemberAST: public PtrOperatorAST
{
public:
    unsigned global_scope_token;
    NestedNameSpecifierAST *nested_name_specifier;
    unsigned star_token;
    SpecifierAST *cv_qualifier_seq;

public:
    virtual PointerToMemberAST *asPointerToMember()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual PointerToMemberAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT PointerAST: public PtrOperatorAST
{
public:
    unsigned star_token;
    SpecifierAST *cv_qualifier_seq;

public:
    virtual PointerAST *asPointer()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual PointerAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ReferenceAST: public PtrOperatorAST
{
public:
    unsigned amp_token;

public:
    virtual ReferenceAST *asReference()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ReferenceAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT BreakStatementAST: public StatementAST
{
public:
    unsigned break_token;
    unsigned semicolon_token;

public:
    virtual BreakStatementAST *asBreakStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BreakStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ContinueStatementAST: public StatementAST
{
public:
    unsigned continue_token;
    unsigned semicolon_token;

public:
    virtual ContinueStatementAST *asContinueStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ContinueStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT GotoStatementAST: public StatementAST
{
public:
    unsigned goto_token;
    unsigned identifier_token;
    unsigned semicolon_token;

public:
    virtual GotoStatementAST *asGotoStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual GotoStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ReturnStatementAST: public StatementAST
{
public:
    unsigned return_token;
    ExpressionAST *expression;
    unsigned semicolon_token;

public:
    virtual ReturnStatementAST *asReturnStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ReturnStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT SizeofExpressionAST: public ExpressionAST
{
public:
    unsigned sizeof_token;
    unsigned lparen_token;
    ExpressionAST *expression;
    unsigned rparen_token;

public:
    virtual SizeofExpressionAST *asSizeofExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SizeofExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NumericLiteralAST: public ExpressionAST
{
public:
    unsigned literal_token;

public:
    virtual NumericLiteralAST *asNumericLiteral()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NumericLiteralAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT BoolLiteralAST: public ExpressionAST
{
public:
    unsigned literal_token;

public:
    virtual BoolLiteralAST *asBoolLiteral()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BoolLiteralAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ThisExpressionAST: public ExpressionAST
{
public:
    unsigned this_token;

public:
    virtual ThisExpressionAST *asThisExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ThisExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT NestedExpressionAST: public ExpressionAST
{
public:
    unsigned lparen_token;
    ExpressionAST *expression;
    unsigned rparen_token;

public:
    virtual NestedExpressionAST *asNestedExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NestedExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT StringLiteralAST: public ExpressionAST
{
public:
    unsigned literal_token;
    StringLiteralAST *next;

public:
    virtual StringLiteralAST *asStringLiteral()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual StringLiteralAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT SwitchStatementAST: public StatementAST
{
public:
    unsigned switch_token;
    unsigned lparen_token;
    ExpressionAST *condition;
    unsigned rparen_token;
    StatementAST *statement;

public: // annotations
    Block *symbol;

public:
    virtual SwitchStatementAST *asSwitchStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SwitchStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TemplateArgumentListAST: public AST
{
public:
    unsigned comma_token;
    ExpressionAST *template_argument;
    TemplateArgumentListAST *next;

public:
    virtual TemplateArgumentListAST *asTemplateArgumentList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TemplateArgumentListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TemplateDeclarationAST: public DeclarationAST
{
public:
    unsigned export_token;
    unsigned template_token;
    unsigned less_token;
    DeclarationListAST *template_parameters;
    unsigned greater_token;
    DeclarationAST *declaration;

public:
    virtual TemplateDeclarationAST *asTemplateDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TemplateDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ThrowExpressionAST: public ExpressionAST
{
public:
    unsigned throw_token;
    ExpressionAST *expression;

public:
    virtual ThrowExpressionAST *asThrowExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ThrowExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TranslationUnitAST: public AST
{
public:
    DeclarationListAST *declarations;

public:
    virtual TranslationUnitAST *asTranslationUnit()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TranslationUnitAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TryBlockStatementAST: public StatementAST
{
public:
    unsigned try_token;
    StatementAST *statement;
    CatchClauseAST *catch_clause_seq;

public:
    virtual TryBlockStatementAST *asTryBlockStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TryBlockStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT CatchClauseAST: public StatementAST
{
public:
    unsigned catch_token;
    unsigned lparen_token;
    ExceptionDeclarationAST *exception_declaration;
    unsigned rparen_token;
    StatementAST *statement;
    CatchClauseAST *next;

public: // annotations
    Block *symbol;

public:
    virtual CatchClauseAST *asCatchClause()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CatchClauseAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TypeIdAST: public ExpressionAST
{
public:
    SpecifierAST *type_specifier;
    DeclaratorAST *declarator;

public:
    virtual TypeIdAST *asTypeId()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypeIdAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TypenameTypeParameterAST: public DeclarationAST
{
public:
    unsigned classkey_token;
    NameAST *name;
    unsigned equal_token;
    ExpressionAST *type_id;

public: // annotations
    Argument *symbol;

public:
    virtual TypenameTypeParameterAST *asTypenameTypeParameter()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypenameTypeParameterAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT TemplateTypeParameterAST: public DeclarationAST
{
public:
    unsigned template_token;
    unsigned less_token;
    DeclarationListAST *template_parameters;
    unsigned greater_token;
    unsigned class_token;
    NameAST *name;
    unsigned equal_token;
    ExpressionAST *type_id;

public:
    Argument *symbol;

public:
    virtual TemplateTypeParameterAST *asTemplateTypeParameter()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TemplateTypeParameterAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT UnaryExpressionAST: public ExpressionAST
{
public:
    unsigned unary_op_token;
    ExpressionAST *expression;

public:
    virtual UnaryExpressionAST *asUnaryExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual UnaryExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT UsingAST: public DeclarationAST
{
public:
    unsigned using_token;
    unsigned typename_token;
    NameAST *name;
    unsigned semicolon_token;

public: // annotations
    UsingDeclaration *symbol;

public:
    virtual UsingAST *asUsing()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual UsingAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT UsingDirectiveAST: public DeclarationAST
{
public:
    unsigned using_token;
    unsigned namespace_token;
    NameAST *name;
    unsigned semicolon_token;

public:
    UsingNamespaceDirective *symbol;

public:
    virtual UsingDirectiveAST *asUsingDirective()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual UsingDirectiveAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT WhileStatementAST: public StatementAST
{
public:
    unsigned while_token;
    unsigned lparen_token;
    ExpressionAST *condition;
    unsigned rparen_token;
    StatementAST *statement;

public: // annotations
    Block *symbol;

public:
    virtual WhileStatementAST *asWhileStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual WhileStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};


// ObjC++
class CPLUSPLUS_EXPORT IdentifierListAST: public AST
{
public:
    NameAST *name;
    unsigned comma_token;
    IdentifierListAST *next;

public:
    virtual IdentifierListAST *asIdentifierList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual IdentifierListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCClassForwardDeclarationAST: public DeclarationAST
{
public:
    SpecifierAST *attributes;
    unsigned class_token;
    IdentifierListAST *identifier_list;
    unsigned semicolon_token;

public: // annotations
    List<ObjCForwardClassDeclaration *> *symbols;

public:
    virtual ObjCClassForwardDeclarationAST *asObjCClassForwardDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCClassForwardDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCClassDeclarationAST: public DeclarationAST
{
public:
    SpecifierAST *attributes;
    unsigned interface_token;
    unsigned implementation_token;
    NameAST *class_name;
    unsigned lparen_token;
    NameAST *category_name;
    unsigned rparen_token;
    unsigned colon_token;
    NameAST *superclass;
    ObjCProtocolRefsAST *protocol_refs;
    ObjCInstanceVariablesDeclarationAST *inst_vars_decl;
    DeclarationListAST *member_declarations;
    unsigned end_token;

public: // annotations
    ObjCClass *symbol;

public:
    virtual ObjCClassDeclarationAST *asObjCClassDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCClassDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCProtocolForwardDeclarationAST: public DeclarationAST
{
public:
    SpecifierAST *attributes;
    unsigned protocol_token;
    IdentifierListAST *identifier_list;
    unsigned semicolon_token;

public: // annotations
    List<ObjCForwardProtocolDeclaration *> *symbols;

public:
    virtual ObjCProtocolForwardDeclarationAST *asObjCProtocolForwardDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCProtocolForwardDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCProtocolDeclarationAST: public DeclarationAST
{
public:
    SpecifierAST *attributes;
    unsigned protocol_token;
    NameAST *name;
    ObjCProtocolRefsAST *protocol_refs;
    DeclarationListAST *member_declarations;
    unsigned end_token;

public: // annotations
    ObjCProtocol *symbol;

public:
    virtual ObjCProtocolDeclarationAST *asObjCProtocolDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCProtocolDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCProtocolRefsAST: public AST
{
public:
    unsigned less_token;
    IdentifierListAST *identifier_list;
    unsigned greater_token;

public:
    virtual ObjCProtocolRefsAST *asObjCProtocolRefs()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCProtocolRefsAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCMessageArgumentAST: public AST
{
public:
    ExpressionAST *parameter_value_expression;

public:
    virtual ObjCMessageArgumentAST *asObjCMessageArgument()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCMessageArgumentAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCMessageArgumentListAST: public AST
{
public:
    ObjCMessageArgumentAST *arg;
    ObjCMessageArgumentListAST *next;

public:
    virtual ObjCMessageArgumentListAST *asObjCMessageArgumentList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCMessageArgumentListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCMessageExpressionAST: public ExpressionAST
{
public:
    unsigned lbracket_token;
    ExpressionAST *receiver_expression;
    ObjCSelectorAST *selector;
    ObjCMessageArgumentListAST *argument_list;
    unsigned rbracket_token;

public:
    virtual ObjCMessageExpressionAST *asObjCMessageExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCMessageExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCProtocolExpressionAST: public ExpressionAST
{
public:
    unsigned protocol_token;
    unsigned lparen_token;
    unsigned identifier_token;
    unsigned rparen_token;

public:
    virtual ObjCProtocolExpressionAST *asObjCProtocolExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCProtocolExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCTypeNameAST: public AST
{
public:
    unsigned lparen_token;
    unsigned type_qualifier;
    ExpressionAST *type_id;
    unsigned rparen_token;

public:
    virtual ObjCTypeNameAST *asObjCTypeName()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCTypeNameAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCEncodeExpressionAST: public ExpressionAST
{
public:
    unsigned encode_token;
    ObjCTypeNameAST *type_name;

public:
    virtual ObjCEncodeExpressionAST *asObjCEncodeExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCEncodeExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCSelectorAST: public AST
{
public: // annotation
    Name *selector_name;

public:
    virtual ObjCSelectorAST *asObjCSelector()
    { return this; }

    virtual ObjCSelectorAST *clone(MemoryPool *pool) const = 0;
};

class CPLUSPLUS_EXPORT ObjCSelectorWithoutArgumentsAST: public ObjCSelectorAST
{
public:
    unsigned name_token;

public:
    virtual ObjCSelectorWithoutArgumentsAST *asObjCSelectorWithoutArguments()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCSelectorWithoutArgumentsAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCSelectorArgumentAST: public AST
{
public:
    unsigned name_token;
    unsigned colon_token;

public:
    virtual ObjCSelectorArgumentAST *asObjCSelectorArgument()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCSelectorArgumentAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCSelectorArgumentListAST: public AST
{
public:
    ObjCSelectorArgumentAST *argument;
    ObjCSelectorArgumentListAST *next;

public:
    virtual ObjCSelectorArgumentListAST *asObjCSelectorArgumentList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCSelectorArgumentListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCSelectorWithArgumentsAST: public ObjCSelectorAST
{
public:
    ObjCSelectorArgumentListAST *selector_arguments;

public:
    virtual ObjCSelectorWithArgumentsAST *asObjCSelectorWithArguments()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCSelectorWithArgumentsAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCSelectorExpressionAST: public ExpressionAST
{
public:
    unsigned selector_token;
    unsigned lparen_token;
    ObjCSelectorAST *selector;
    unsigned rparen_token;

public:
    virtual ObjCSelectorExpressionAST *asObjCSelectorExpression()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCSelectorExpressionAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCInstanceVariablesDeclarationAST: public AST
{
public:
    unsigned lbrace_token;
    DeclarationListAST *instance_variables;
    unsigned rbrace_token;

public:
    virtual ObjCInstanceVariablesDeclarationAST *asObjCInstanceVariablesDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCInstanceVariablesDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCVisibilityDeclarationAST: public DeclarationAST
{
public:
    unsigned visibility_token;

public:
    virtual ObjCVisibilityDeclarationAST *asObjCVisibilityDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCVisibilityDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCPropertyAttributeAST: public AST
{
public:
    unsigned attribute_identifier_token;
    unsigned equals_token;
    ObjCSelectorAST *method_selector;

public:
    virtual ObjCPropertyAttributeAST *asObjCPropertyAttribute()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCPropertyAttributeAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCPropertyAttributeListAST: public AST
{
public:
    ObjCPropertyAttributeAST *attr;
    unsigned comma_token;
    ObjCPropertyAttributeListAST *next;

public:
    virtual ObjCPropertyAttributeListAST *asObjCPropertyAttributeList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCPropertyAttributeListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCPropertyDeclarationAST: public DeclarationAST
{
public:
    SpecifierAST *attributes;
    unsigned property_token;
    unsigned lparen_token;
    ObjCPropertyAttributeListAST *property_attributes;
    unsigned rparen_token;
    DeclarationAST *simple_declaration;

public:
    virtual ObjCPropertyDeclarationAST *asObjCPropertyDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCPropertyDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCMessageArgumentDeclarationAST: public NameAST
{
public:
    ObjCTypeNameAST* type_name;
    SpecifierAST *attributes;
    unsigned param_name_token;

public: // annotations
    Argument *argument;

public:
    virtual ObjCMessageArgumentDeclarationAST *asObjCMessageArgumentDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCMessageArgumentDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCMessageArgumentDeclarationListAST: public AST
{
public:
    ObjCMessageArgumentDeclarationAST *argument_declaration;
    ObjCMessageArgumentDeclarationListAST *next;

public:
    virtual ObjCMessageArgumentDeclarationListAST *asObjCMessageArgumentDeclarationList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCMessageArgumentDeclarationListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCMethodPrototypeAST: public AST
{
public:
    unsigned method_type_token;
    ObjCTypeNameAST *type_name;
    ObjCSelectorAST *selector;
    ObjCMessageArgumentDeclarationListAST *arguments;
    SpecifierAST *attributes;

public: // annotations
    ObjCMethod *symbol;

public:
    virtual ObjCMethodPrototypeAST *asObjCMethodPrototype()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCMethodPrototypeAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCMethodDeclarationAST: public DeclarationAST
{
public:
    ObjCMethodPrototypeAST *method_prototype;
    StatementAST *function_body;
    unsigned semicolon_token;

public:
    virtual ObjCMethodDeclarationAST *asObjCMethodDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCMethodDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCSynthesizedPropertyAST: public AST
{
public:
    unsigned property_identifier;
    unsigned equals_token;
    unsigned property_alias_identifier;

public:
    virtual ObjCSynthesizedPropertyAST *asObjCSynthesizedProperty()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCSynthesizedPropertyAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCSynthesizedPropertyListAST: public AST
{
public:
    ObjCSynthesizedPropertyAST *synthesized_property;
    unsigned comma_token;
    ObjCSynthesizedPropertyListAST *next;

public:
    virtual ObjCSynthesizedPropertyListAST *asObjCSynthesizedPropertyList()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCSynthesizedPropertyListAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCSynthesizedPropertiesDeclarationAST: public DeclarationAST
{
public:
    unsigned synthesized_token;
    ObjCSynthesizedPropertyListAST *property_identifiers;
    unsigned semicolon_token;

public:
    virtual ObjCSynthesizedPropertiesDeclarationAST *asObjCSynthesizedPropertiesDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCSynthesizedPropertiesDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCDynamicPropertiesDeclarationAST: public DeclarationAST
{
public:
    unsigned dynamic_token;
    IdentifierListAST *property_identifiers;
    unsigned semicolon_token;

public:
    virtual ObjCDynamicPropertiesDeclarationAST *asObjCDynamicPropertiesDeclaration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCDynamicPropertiesDeclarationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCFastEnumerationAST: public StatementAST
{
public:
    unsigned for_token;
    unsigned lparen_token;

    // declaration
    SpecifierAST *type_specifiers;
    DeclaratorAST *declarator;
    // or an expression
    ExpressionAST *initializer;

    unsigned in_token;
    ExpressionAST *fast_enumeratable_expression;
    unsigned rparen_token;
    StatementAST *body_statement;

public: // annotations
    Block *symbol;

public:
    virtual ObjCFastEnumerationAST *asObjCFastEnumeration()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCFastEnumerationAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

class CPLUSPLUS_EXPORT ObjCSynchronizedStatementAST: public StatementAST
{
public:
    unsigned synchronized_token;
    unsigned lparen_token;
    ExpressionAST *synchronized_object;
    unsigned rparen_token;
    StatementAST *statement;

public:
    virtual ObjCSynchronizedStatementAST *asObjCSynchronizedStatement()
    { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ObjCSynchronizedStatementAST *clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
};

CPLUSPLUS_END_NAMESPACE
CPLUSPLUS_END_HEADER

#endif // CPLUSPLUS_AST_H
