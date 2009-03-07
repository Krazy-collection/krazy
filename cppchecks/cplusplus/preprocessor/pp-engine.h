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
/*
  Copyright 2005 Roberto Raggi <roberto@kdevelop.org>

  Permission to use, copy, modify, distribute, and sell this software and its
  documentation for any purpose is hereby granted without fee, provided that
  the above copyright notice appear in all copies and that both that
  copyright notice and this permission notice appear in supporting
  documentation.

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  KDEVELOP TEAM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef CPLUSPLUS_PP_ENGINE_H
#define CPLUSPLUS_PP_ENGINE_H

#include "PreprocessorClient.h"
#include "pp-macro-expander.h"

#include <Token.h>
#include <QtCore/QVector>

namespace CPlusPlus {
    class Token;
}

namespace CPlusPlus {

struct Value;

class CPP_PREPROCESSOR_EXPORT Preprocessor
{
public:
    Preprocessor(Client *client, Environment *env);

    QByteArray operator()(const QByteArray &filename,
                          const QByteArray &source);

private:
    enum { MAX_LEVEL = 512 };

    enum PP_DIRECTIVE_TYPE
    {
        PP_UNKNOWN_DIRECTIVE,
        PP_DEFINE,
        PP_IMPORT,
        PP_INCLUDE,
        PP_INCLUDE_NEXT,
        PP_ELIF,
        PP_ELSE,
        PP_ENDIF,
        PP_IF,
        PP_IFDEF,
        PP_IFNDEF,
        PP_UNDEF
    };

    typedef const CPlusPlus::Token *TokenIterator;

    struct State {
        QByteArray source;
        QVector<CPlusPlus::Token> tokens;
        TokenIterator dot;
    };

    bool markGeneratedTokens(bool markGeneratedTokens, TokenIterator dot = 0);

    void preprocess(const QByteArray &filename,
                    const QByteArray &source,
                    QByteArray *result);

    QByteArray expand(const QByteArray &source);
    void expand(const QByteArray &source, QByteArray *result);
    void expand(const char *first, const char *last, QByteArray *result);
    void expandBuiltinMacro(TokenIterator identifierToken,
                            const QByteArray &spell);
    void expandObjectLikeMacro(TokenIterator identifierToken,
                               const QByteArray &spell,
                               Macro *m, QByteArray *result);
    void expandFunctionLikeMacro(TokenIterator identifierToken, Macro *m,
                                 const QVector<MacroArgumentReference> &actuals);

    void resetIfLevel();
    bool testIfLevel();
    int skipping() const;

    PP_DIRECTIVE_TYPE classifyDirective(const QByteArray &directive) const;

    Value evalExpression(TokenIterator firstToken,
                         TokenIterator lastToken,
                         const QByteArray &source) const;

    QVector<CPlusPlus::Token> tokenize(const QByteArray &text) const;

    const char *startOfToken(const CPlusPlus::Token &token) const;
    const char *endOfToken(const CPlusPlus::Token &token) const;

    QByteArray tokenSpell(const CPlusPlus::Token &token) const;
    QByteArray tokenText(const CPlusPlus::Token &token) const; // does a deep copy

    void collectActualArguments(QVector<MacroArgumentReference> *actuals);
    MacroArgumentReference collectOneActualArgument();

    void processNewline(bool force = false);

    void processSkippingBlocks(bool skippingBlocks,
                               TokenIterator dot, TokenIterator lastToken);

    Macro *processObjectLikeMacro(TokenIterator identifierToken,
                                  const QByteArray &spell,
                                  Macro *m);

    void processDirective(TokenIterator dot, TokenIterator lastToken);
    void processInclude(bool skipCurrentPath,
                        TokenIterator dot, TokenIterator lastToken,
                        bool acceptMacros = true);
    void processDefine(TokenIterator dot, TokenIterator lastToken);
    void processIf(TokenIterator dot, TokenIterator lastToken);
    void processElse(TokenIterator dot, TokenIterator lastToken);
    void processElif(TokenIterator dot, TokenIterator lastToken);
    void processEndif(TokenIterator dot, TokenIterator lastToken);
    void processIfdef(bool checkUndefined,
                      TokenIterator dot, TokenIterator lastToken);
    void processUndef(TokenIterator dot, TokenIterator lastToken);

    bool isQtReservedWord(const QByteArray &name) const;

    State state() const;
    void pushState(const State &state);
    void popState();

    State createStateFromSource(const QByteArray &source) const;

private:
    Client *client;
    Environment *env;
    MacroExpander _expand;

    bool _skipping[MAX_LEVEL]; // ### move in state
    bool _true_test[MAX_LEVEL]; // ### move in state
    int iflevel; // ### move in state

    QList<State> _savedStates;

    QByteArray _source;
    QVector<CPlusPlus::Token> _tokens;
    TokenIterator _dot;

    QByteArray *_result;
    bool _markGeneratedTokens;
};

} // namespace CPlusPlus

#endif // CPLUSPLUS_PP_ENGINE_H
