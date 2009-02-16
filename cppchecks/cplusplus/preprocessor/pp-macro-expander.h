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

#ifndef PP_MACRO_EXPANDER_H
#define PP_MACRO_EXPANDER_H

#include "pp-scanner.h"

namespace CPlusPlus {

    struct pp_frame
    {
        Macro *expanding_macro;
        const QVector<QByteArray> actuals;

        pp_frame (Macro *expanding_macro, const QVector<QByteArray> &actuals)
            : expanding_macro (expanding_macro),
              actuals (actuals)
        { }
    };

    class MacroExpander
    {
        Environment &env;
        pp_frame *frame;

        pp_skip_number skip_number;
        pp_skip_identifier skip_identifier;
        pp_skip_string_literal skip_string_literal;
        pp_skip_char_literal skip_char_literal;
        pp_skip_argument skip_argument;
        pp_skip_comment_or_divop skip_comment_or_divop;
        pp_skip_blanks skip_blanks;
        pp_skip_whitespaces skip_whitespaces;

        const QByteArray *resolve_formal (const QByteArray &name);

    public:
        MacroExpander (Environment &env, pp_frame *frame = 0);

        const char *operator () (const char *first, const char *last,
                                 QByteArray *result);

        const char *operator () (const QByteArray &source,
                                 QByteArray *result)
        { return operator()(source.constBegin(), source.constEnd(), result); }

        const char *skip_argument_variadics (const QVector<QByteArray> &actuals,
                                             Macro *macro,
                                             const char *first, const char *last);

    public: // attributes
        int lines;
        int generated_lines;
    };

} // namespace CPlusPlus

#endif // PP_MACRO_EXPANDER_H

