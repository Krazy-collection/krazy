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

#ifndef CPPDOCUMENT_H
#define CPPDOCUMENT_H

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "Macro.h"

namespace CPlusPlus {

class Macro;

class CPP_PREPROCESSOR_EXPORT Document
{
public:
    typedef QSharedPointer<Document> Ptr;

    class DiagnosticMessage
    {
      int _level;
      QString _fileName;
      unsigned _line;
      unsigned _column;
      QString _text;

      public:
        enum Level {
            Warning,
            Error,
            Fatal
        };

      public:
        DiagnosticMessage(int level, const QString &fileName,
                          int line, int column,
                          const QString &text)
          : _level(level),
            _fileName(fileName),
            _line(line),
            _column(column),
            _text(text)
        { }

        int level() const
        { return _level; }

        bool isWarning() const
        { return _level == Warning; }

        bool isError() const
        { return _level == Error; }

        bool isFatal() const
        { return _level == Fatal; }

        QString fileName() const
        { return _fileName; }

        unsigned line() const
        { return _line; }

        unsigned column() const
        { return _column; }

        QString text() const
        { return _text; }
    };

    class Block
    {
      unsigned _begin;
      unsigned _end;

      public:
        inline Block(unsigned begin = 0, unsigned end = 0)
            : _begin(begin), _end(end)
        { }

        inline unsigned begin() const
        { return _begin; }

        inline unsigned end() const
        { return _end; }

        bool contains(unsigned pos) const
        { return pos >= _begin && pos < _end; }
    };

    class Include {
      Ptr _document;
      unsigned _line;

      public:
        Include(Ptr &document, unsigned line)
            : _document(document), _line(line)
        { }

        Ptr document() const
        { return _document; }

        unsigned line() const
        { return _line; }
    };

    class MacroUse: public Block {
      Macro _macro;

      public:
        inline MacroUse(const Macro &macro,
                        unsigned begin = 0,
                        unsigned end = 0)
          : Block(begin, end),
            _macro(macro)
        {}

        const Macro &macro() const
        { return _macro; }
    };

public:
    void addDiagnosticMessage(const DiagnosticMessage &d)
    { _diagnosticMessages.append(d); }

    void addMacroUse(const Macro &macro, unsigned offset, unsigned length);

    Ptr addIncludeFile(Ptr parent, const QString &fileName, unsigned line);

    void appendMacro(const Macro &macro);

    static Ptr create(Ptr parent, const QString &fileName);

    QList<Macro> definedMacros() const
    { return _definedMacros; }

    QList<DiagnosticMessage> diagnosticMessages() const
    { return _diagnosticMessages; }

    /**
     * Returns the name of this document.
     */
    QString fileName() const;

    QList<Include> includes() const
    { return _includes; }

    Ptr parent() const
    { return _parent; }

    void setSource(const QByteArray &source);

    QByteArray source() const
    { return _source; }

    QList<Block> skippedBlocks() const
    { return _skippedBlocks; }

    void startSkippingBlocks(unsigned offset);

    void stopSkippingBlocks(unsigned offset);

    QList<MacroUse> macroUses() const
    { return _macroUses; }

  private: // Functions
    Document(const Document &other);

    void operator =(const Document &other);

    Document(const QString &fileName, Ptr parent);

  private: // Members
    QString _fileName;
    QList<Macro> _definedMacros;
    QList<DiagnosticMessage> _diagnosticMessages;
    QList<Include> _includes;
    QList<MacroUse> _macroUses;
    Ptr _parent;
    QList<Block> _skippedBlocks;
    QByteArray _source;
};

} // end of namespace CPlusPlus

#endif // CPPDOCUMENT_H
