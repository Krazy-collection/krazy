/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
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
** contact the sales department at qt-sales@nokia.com.
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

#ifndef CPLUSPLUS_CHECKSTATEMENT_H
#define CPLUSPLUS_CHECKSTATEMENT_H

#include "CPlusPlusForwardDeclarations.h"
#include "SemanticCheck.h"

CPLUSPLUS_BEGIN_HEADER
CPLUSPLUS_BEGIN_NAMESPACE

class CPLUSPLUS_EXPORT CheckStatement: public SemanticCheck
{
public:
    CheckStatement(Semantic *semantic);
    virtual ~CheckStatement();

    void check(StatementAST *statement, Scope *scope);

protected:
    StatementAST *switchStatement(StatementAST *statement);
    Scope *switchScope(Scope *scope);

    using ASTVisitor::visit;

    virtual bool visit(CaseStatementAST *ast);
    virtual bool visit(CompoundStatementAST *ast);
    virtual bool visit(DeclarationStatementAST *ast);
    virtual bool visit(DoStatementAST *ast);
    virtual bool visit(ExpressionOrDeclarationStatementAST *ast);
    virtual bool visit(ExpressionStatementAST *ast);
    virtual bool visit(ForStatementAST *ast);
    virtual bool visit(IfStatementAST *ast);
    virtual bool visit(LabeledStatementAST *ast);
    virtual bool visit(BreakStatementAST *ast);
    virtual bool visit(ContinueStatementAST *ast);
    virtual bool visit(GotoStatementAST *ast);
    virtual bool visit(ReturnStatementAST *ast);
    virtual bool visit(SwitchStatementAST *ast);
    virtual bool visit(TryBlockStatementAST *ast);
    virtual bool visit(CatchClauseAST *ast);
    virtual bool visit(WhileStatementAST *ast);

private:
    StatementAST *_statement;
    Scope *_scope;
};

CPLUSPLUS_END_NAMESPACE
CPLUSPLUS_END_HEADER

#endif // CPLUSPLUS_CHECKSTATEMENT_H
