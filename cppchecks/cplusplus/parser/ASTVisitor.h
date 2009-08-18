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

#ifndef CPLUSPLUS_ASTVISITOR_H
#define CPLUSPLUS_ASTVISITOR_H

#include "CPlusPlusForwardDeclarations.h"
#include "ASTfwd.h"

CPLUSPLUS_BEGIN_HEADER
CPLUSPLUS_BEGIN_NAMESPACE

class CPLUSPLUS_EXPORT ASTVisitor
{
    ASTVisitor(const ASTVisitor &other);
    void operator =(const ASTVisitor &other);

public:
    ASTVisitor(Control *control);
    virtual ~ASTVisitor();

    Control *control() const;
    TranslationUnit *translationUnit() const;

    unsigned tokenCount() const;
    const Token &tokenAt(unsigned index) const;
    int tokenKind(unsigned index) const;
    const char *spell(unsigned index) const;
    Identifier *identifier(unsigned index) const;
    Literal *literal(unsigned index) const;
    NumericLiteral *numericLiteral(unsigned index) const;
    StringLiteral *stringLiteral(unsigned index) const;

    void getPosition(unsigned offset,
                     unsigned *line,
                     unsigned *column = 0,
                     StringLiteral **fileName = 0) const;

    void getTokenPosition(unsigned index,
                          unsigned *line,
                          unsigned *column = 0,
                          StringLiteral **fileName = 0) const;

    void getTokenStartPosition(unsigned index, unsigned *line, unsigned *column) const;
    void getTokenEndPosition(unsigned index, unsigned *line, unsigned *column) const;

    void accept(AST *ast);

    virtual bool preVisit(AST *) { return true; }
    virtual void postVisit(AST *) {}

    virtual bool visit(AccessDeclarationAST *) { return true; }
    virtual bool visit(ArrayAccessAST *) { return true; }
    virtual bool visit(ArrayDeclaratorAST *) { return true; }
    virtual bool visit(ArrayInitializerAST *) { return true; }
    virtual bool visit(AsmDefinitionAST *) { return true; }
    virtual bool visit(AttributeSpecifierAST *) { return true; }
    virtual bool visit(AttributeAST *) { return true; }
    virtual bool visit(BaseSpecifierAST *) { return true; }
    virtual bool visit(BinaryExpressionAST *) { return true; }
    virtual bool visit(BoolLiteralAST *) { return true; }
    virtual bool visit(BreakStatementAST *) { return true; }
    virtual bool visit(CallAST *) { return true; }
    virtual bool visit(CaseStatementAST *) { return true; }
    virtual bool visit(CastExpressionAST *) { return true; }
    virtual bool visit(CatchClauseAST *) { return true; }
    virtual bool visit(ClassSpecifierAST *) { return true; }
    virtual bool visit(CompoundLiteralAST *) { return true; }
    virtual bool visit(CompoundStatementAST *) { return true; }
    virtual bool visit(ConditionAST *) { return true; }
    virtual bool visit(ConditionalExpressionAST *) { return true; }
    virtual bool visit(ContinueStatementAST *) { return true; }
    virtual bool visit(ConversionFunctionIdAST *) { return true; }
    virtual bool visit(CppCastExpressionAST *) { return true; }
    virtual bool visit(CtorInitializerAST *) { return true; }
    virtual bool visit(DeclaratorAST *) { return true; }
    virtual bool visit(DeclarationStatementAST *) { return true; }
    virtual bool visit(DeclaratorIdAST *) { return true; }
    virtual bool visit(DeclaratorListAST *) { return true; }
    virtual bool visit(DeleteExpressionAST *) { return true; }
    virtual bool visit(DestructorNameAST *) { return true; }
    virtual bool visit(DoStatementAST *) { return true; }
    virtual bool visit(ElaboratedTypeSpecifierAST *) { return true; }
    virtual bool visit(EmptyDeclarationAST *) { return true; }
    virtual bool visit(EnumSpecifierAST *) { return true; }
    virtual bool visit(EnumeratorAST *) { return true; }
    virtual bool visit(ExceptionDeclarationAST *) { return true; }
    virtual bool visit(ExceptionSpecificationAST *) { return true; }
    virtual bool visit(ExpressionListAST *) { return true; }
    virtual bool visit(ExpressionOrDeclarationStatementAST *) { return true; }
    virtual bool visit(ExpressionStatementAST *) { return true; }
    virtual bool visit(ForeachStatementAST *) { return true; }
    virtual bool visit(ForStatementAST *) { return true; }
    virtual bool visit(FunctionDeclaratorAST *) { return true; }
    virtual bool visit(FunctionDefinitionAST *) { return true; }
    virtual bool visit(GotoStatementAST *) { return true; }
    virtual bool visit(IfStatementAST *) { return true; }
    virtual bool visit(LabeledStatementAST *) { return true; }
    virtual bool visit(LinkageBodyAST *) { return true; }
    virtual bool visit(LinkageSpecificationAST *) { return true; }
    virtual bool visit(MemInitializerAST *) { return true; }
    virtual bool visit(MemberAccessAST *) { return true; }
    virtual bool visit(NamedTypeSpecifierAST *) { return true; }
    virtual bool visit(NamespaceAST *) { return true; }
    virtual bool visit(NamespaceAliasDefinitionAST *) { return true; }
    virtual bool visit(NestedDeclaratorAST *) { return true; }
    virtual bool visit(NestedExpressionAST *) { return true; }
    virtual bool visit(NestedNameSpecifierAST *) { return true; }
    virtual bool visit(NewPlacementAST *) { return true; }
    virtual bool visit(NewArrayDeclaratorAST *) { return true; }
    virtual bool visit(NewExpressionAST *) { return true; }
    virtual bool visit(NewInitializerAST *) { return true; }
    virtual bool visit(NewTypeIdAST *) { return true; }
    virtual bool visit(NumericLiteralAST *) { return true; }
    virtual bool visit(OperatorAST *) { return true; }
    virtual bool visit(OperatorFunctionIdAST *) { return true; }
    virtual bool visit(ParameterDeclarationAST *) { return true; }
    virtual bool visit(ParameterDeclarationClauseAST *) { return true; }
    virtual bool visit(PointerAST *) { return true; }
    virtual bool visit(PointerToMemberAST *) { return true; }
    virtual bool visit(PostIncrDecrAST *) { return true; }
    virtual bool visit(PostfixExpressionAST *) { return true; }
    virtual bool visit(QualifiedNameAST *) { return true; }
    virtual bool visit(ReferenceAST *) { return true; }
    virtual bool visit(ReturnStatementAST *) { return true; }
    virtual bool visit(SimpleDeclarationAST *) { return true; }
    virtual bool visit(SimpleNameAST *) { return true; }
    virtual bool visit(SimpleSpecifierAST *) { return true; }
    virtual bool visit(SizeofExpressionAST *) { return true; }
    virtual bool visit(StatementListAST *) { return true; }
    virtual bool visit(StringLiteralAST *) { return true; }
    virtual bool visit(SwitchStatementAST *) { return true; }
    virtual bool visit(TemplateArgumentListAST *) { return true; }
    virtual bool visit(TemplateDeclarationAST *) { return true; }
    virtual bool visit(TemplateIdAST *) { return true; }
    virtual bool visit(TemplateTypeParameterAST *) { return true; }
    virtual bool visit(ThisExpressionAST *) { return true; }
    virtual bool visit(ThrowExpressionAST *) { return true; }
    virtual bool visit(TranslationUnitAST *) { return true; }
    virtual bool visit(TryBlockStatementAST *) { return true; }
    virtual bool visit(TypeConstructorCallAST *) { return true; }
    virtual bool visit(TypeIdAST *) { return true; }
    virtual bool visit(TypeidExpressionAST *) { return true; }
    virtual bool visit(TypeofSpecifierAST *) { return true; }
    virtual bool visit(TypenameCallExpressionAST *) { return true; }
    virtual bool visit(TypenameTypeParameterAST *) { return true; }
    virtual bool visit(UnaryExpressionAST *) { return true; }
    virtual bool visit(UsingAST *) { return true; }
    virtual bool visit(UsingDirectiveAST *) { return true; }
    virtual bool visit(WhileStatementAST *) { return true; }
    virtual bool visit(QtMethodAST *) { return true; }

    // ObjC++
    virtual bool visit(IdentifierListAST *) { return true; }
    virtual bool visit(ObjCClassDeclarationAST *) { return true; }
    virtual bool visit(ObjCClassForwardDeclarationAST *) { return true; }
    virtual bool visit(ObjCProtocolDeclarationAST *) { return true; }
    virtual bool visit(ObjCProtocolForwardDeclarationAST *) { return true; }
    virtual bool visit(ObjCProtocolRefsAST *) { return true; }
    virtual bool visit(ObjCMessageExpressionAST *) { return true; }
    virtual bool visit(ObjCMessageArgumentListAST *) { return true; }
    virtual bool visit(ObjCMessageArgumentAST *) { return true; }
    virtual bool visit(ObjCProtocolExpressionAST *) { return true; }
    virtual bool visit(ObjCTypeNameAST *) { return true; }
    virtual bool visit(ObjCEncodeExpressionAST *) { return true; }
    virtual bool visit(ObjCSelectorWithoutArgumentsAST *) { return true; }
    virtual bool visit(ObjCSelectorArgumentAST *) { return true; }
    virtual bool visit(ObjCSelectorArgumentListAST *) { return true; }
    virtual bool visit(ObjCSelectorWithArgumentsAST *) { return true; }
    virtual bool visit(ObjCSelectorExpressionAST *) { return true; }
    virtual bool visit(ObjCInstanceVariablesDeclarationAST *) { return true; }
    virtual bool visit(ObjCVisibilityDeclarationAST *) { return true; }
    virtual bool visit(ObjCPropertyAttributeAST *) { return true; }
    virtual bool visit(ObjCPropertyAttributeListAST *) { return true; }
    virtual bool visit(ObjCPropertyDeclarationAST *) { return true; }
    virtual bool visit(ObjCMethodPrototypeAST *) { return true; }
    virtual bool visit(ObjCMethodDeclarationAST *) { return true; }
    virtual bool visit(ObjCMessageArgumentDeclarationListAST *) { return true; }
    virtual bool visit(ObjCMessageArgumentDeclarationAST *) { return true; }
    virtual bool visit(ObjCSynthesizedPropertyAST *) { return true; }
    virtual bool visit(ObjCSynthesizedPropertyListAST *) { return true; }
    virtual bool visit(ObjCSynthesizedPropertiesDeclarationAST *) { return true; }
    virtual bool visit(ObjCDynamicPropertiesDeclarationAST *) { return true; }
    virtual bool visit(ObjCFastEnumerationAST *) { return true; }
    virtual bool visit(ObjCSynchronizedStatementAST *) { return true; }

    virtual bool visit(DeclarationListAST *) { return true; }
    virtual void endVisit(DeclarationListAST *) { }

    virtual void endVisit(AccessDeclarationAST *) { }
    virtual void endVisit(ArrayAccessAST *) { }
    virtual void endVisit(ArrayDeclaratorAST *) { }
    virtual void endVisit(ArrayInitializerAST *) { }
    virtual void endVisit(AsmDefinitionAST *) { }
    virtual void endVisit(AttributeSpecifierAST *) { }
    virtual void endVisit(AttributeAST *) { }
    virtual void endVisit(BaseSpecifierAST *) { }
    virtual void endVisit(BinaryExpressionAST *) { }
    virtual void endVisit(BoolLiteralAST *) { }
    virtual void endVisit(BreakStatementAST *) { }
    virtual void endVisit(CallAST *) { }
    virtual void endVisit(CaseStatementAST *) { }
    virtual void endVisit(CastExpressionAST *) { }
    virtual void endVisit(CatchClauseAST *) { }
    virtual void endVisit(ClassSpecifierAST *) { }
    virtual void endVisit(CompoundLiteralAST *) { }
    virtual void endVisit(CompoundStatementAST *) { }
    virtual void endVisit(ConditionAST *) { }
    virtual void endVisit(ConditionalExpressionAST *) { }
    virtual void endVisit(ContinueStatementAST *) { }
    virtual void endVisit(ConversionFunctionIdAST *) { }
    virtual void endVisit(CppCastExpressionAST *) { }
    virtual void endVisit(CtorInitializerAST *) { }
    virtual void endVisit(DeclaratorAST *) { }
    virtual void endVisit(DeclarationStatementAST *) { }
    virtual void endVisit(DeclaratorIdAST *) { }
    virtual void endVisit(DeclaratorListAST *) { }
    virtual void endVisit(DeleteExpressionAST *) { }
    virtual void endVisit(DestructorNameAST *) { }
    virtual void endVisit(DoStatementAST *) { }
    virtual void endVisit(ElaboratedTypeSpecifierAST *) { }
    virtual void endVisit(EmptyDeclarationAST *) { }
    virtual void endVisit(EnumSpecifierAST *) { }
    virtual void endVisit(EnumeratorAST *) { }
    virtual void endVisit(ExceptionDeclarationAST *) { }
    virtual void endVisit(ExceptionSpecificationAST *) { }
    virtual void endVisit(ExpressionListAST *) { }
    virtual void endVisit(ExpressionOrDeclarationStatementAST *) { }
    virtual void endVisit(ExpressionStatementAST *) { }
    virtual void endVisit(ForeachStatementAST *) { }
    virtual void endVisit(ForStatementAST *) { }
    virtual void endVisit(FunctionDeclaratorAST *) { }
    virtual void endVisit(FunctionDefinitionAST *) { }
    virtual void endVisit(GotoStatementAST *) { }
    virtual void endVisit(IfStatementAST *) { }
    virtual void endVisit(LabeledStatementAST *) { }
    virtual void endVisit(LinkageBodyAST *) { }
    virtual void endVisit(LinkageSpecificationAST *) { }
    virtual void endVisit(MemInitializerAST *) { }
    virtual void endVisit(MemberAccessAST *) { }
    virtual void endVisit(NamedTypeSpecifierAST *) { }
    virtual void endVisit(NamespaceAST *) { }
    virtual void endVisit(NamespaceAliasDefinitionAST *) { }
    virtual void endVisit(NestedDeclaratorAST *) { }
    virtual void endVisit(NestedExpressionAST *) { }
    virtual void endVisit(NestedNameSpecifierAST *) { }
    virtual void endVisit(NewPlacementAST *) { }
    virtual void endVisit(NewArrayDeclaratorAST *) { }
    virtual void endVisit(NewExpressionAST *) { }
    virtual void endVisit(NewInitializerAST *) { }
    virtual void endVisit(NewTypeIdAST *) { }
    virtual void endVisit(NumericLiteralAST *) { }
    virtual void endVisit(OperatorAST *) { }
    virtual void endVisit(OperatorFunctionIdAST *) { }
    virtual void endVisit(ParameterDeclarationAST *) { }
    virtual void endVisit(ParameterDeclarationClauseAST *) { }
    virtual void endVisit(PointerAST *) { }
    virtual void endVisit(PointerToMemberAST *) { }
    virtual void endVisit(PostIncrDecrAST *) { }
    virtual void endVisit(PostfixExpressionAST *) { }
    virtual void endVisit(QualifiedNameAST *) { }
    virtual void endVisit(ReferenceAST *) { }
    virtual void endVisit(ReturnStatementAST *) { }
    virtual void endVisit(SimpleDeclarationAST *) { }
    virtual void endVisit(SimpleNameAST *) { }
    virtual void endVisit(SimpleSpecifierAST *) { }
    virtual void endVisit(SizeofExpressionAST *) { }
    virtual void endVisit(StatementListAST *) { }
    virtual void endVisit(StringLiteralAST *) { }
    virtual void endVisit(SwitchStatementAST *) { }
    virtual void endVisit(TemplateArgumentListAST *) { }
    virtual void endVisit(TemplateDeclarationAST *) { }
    virtual void endVisit(TemplateIdAST *) { }
    virtual void endVisit(TemplateTypeParameterAST *) { }
    virtual void endVisit(ThisExpressionAST *) { }
    virtual void endVisit(ThrowExpressionAST *) { }
    virtual void endVisit(TranslationUnitAST *) { }
    virtual void endVisit(TryBlockStatementAST *) { }
    virtual void endVisit(TypeConstructorCallAST *) { }
    virtual void endVisit(TypeIdAST *) { }
    virtual void endVisit(TypeidExpressionAST *) { }
    virtual void endVisit(TypeofSpecifierAST *) { }
    virtual void endVisit(TypenameCallExpressionAST *) { }
    virtual void endVisit(TypenameTypeParameterAST *) { }
    virtual void endVisit(UnaryExpressionAST *) { }
    virtual void endVisit(UsingAST *) { }
    virtual void endVisit(UsingDirectiveAST *) { }
    virtual void endVisit(WhileStatementAST *) { }
    virtual void endVisit(QtMethodAST *) { }

    // ObjC++
    virtual void endVisit(IdentifierListAST *) { }
    virtual void endVisit(ObjCClassDeclarationAST *) { }
    virtual void endVisit(ObjCClassForwardDeclarationAST *) { }
    virtual void endVisit(ObjCProtocolDeclarationAST *) { }
    virtual void endVisit(ObjCProtocolForwardDeclarationAST *) { }
    virtual void endVisit(ObjCProtocolRefsAST *) { }
    virtual void endVisit(ObjCMessageExpressionAST *) { }
    virtual void endVisit(ObjCMessageArgumentListAST *) { }
    virtual void endVisit(ObjCMessageArgumentAST *) { }
    virtual void endVisit(ObjCProtocolExpressionAST *) { }
    virtual void endVisit(ObjCTypeNameAST *) { }
    virtual void endVisit(ObjCEncodeExpressionAST *) { }
    virtual void endVisit(ObjCSelectorWithoutArgumentsAST *) { }
    virtual void endVisit(ObjCSelectorArgumentAST *) { }
    virtual void endVisit(ObjCSelectorArgumentListAST *) { }
    virtual void endVisit(ObjCSelectorWithArgumentsAST *) { }
    virtual void endVisit(ObjCSelectorExpressionAST *) { }
    virtual void endVisit(ObjCInstanceVariablesDeclarationAST *) { }
    virtual void endVisit(ObjCVisibilityDeclarationAST *) { }
    virtual void endVisit(ObjCPropertyAttributeAST *) { }
    virtual void endVisit(ObjCPropertyAttributeListAST *) { }
    virtual void endVisit(ObjCPropertyDeclarationAST *) { }
    virtual void endVisit(ObjCMethodPrototypeAST *) { }
    virtual void endVisit(ObjCMethodDeclarationAST *) { }
    virtual void endVisit(ObjCMessageArgumentDeclarationListAST *) { }
    virtual void endVisit(ObjCMessageArgumentDeclarationAST *) { }
    virtual void endVisit(ObjCSynthesizedPropertyAST *) { }
    virtual void endVisit(ObjCSynthesizedPropertyListAST *) { }
    virtual void endVisit(ObjCSynthesizedPropertiesDeclarationAST *) { }
    virtual void endVisit(ObjCDynamicPropertiesDeclarationAST *) { }
    virtual void endVisit(ObjCFastEnumerationAST *) { }
    virtual void endVisit(ObjCSynchronizedStatementAST *) { }

private:
    Control *_control;
};

CPLUSPLUS_END_NAMESPACE
CPLUSPLUS_END_HEADER

#endif // CPLUSPLUS_ASTVISITOR_H
