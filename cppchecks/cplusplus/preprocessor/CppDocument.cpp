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

#include "CppDocument.h"

#include <AST.h>
#include <Control.h>
#include <DiagnosticClient.h>
#include <Literals.h>
#include <Scope.h>
#include <Semantic.h>
#include <Symbols.h>
#include <TranslationUnit.h>

#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QtDebug>

#include "PreprocessorClient.h"

using namespace CPlusPlus;

namespace {

class DocumentDiagnosticClient : public DiagnosticClient
{
    enum { MAX_MESSAGE_COUNT = 10 };

public:
    DocumentDiagnosticClient(Document *doc, QList<Document::DiagnosticMessage> *messages)
        : doc(doc),
          messages(messages)
    { }

    virtual void report(int level,
                        StringLiteral *fileId,
                        unsigned line, unsigned column,
                        const char *format, va_list ap)
    {
        if (messages->count() == MAX_MESSAGE_COUNT)
            return;

        const QString fileName = QString::fromUtf8(fileId->chars(), fileId->size());

        if (fileName != doc->fileName())
            return;

        QString message;
        message.vsprintf(format, ap);

        Document::DiagnosticMessage m(convertLevel(level), doc->fileName(),
                                      line, column, message);
        messages->append(m);
    }

    static int convertLevel(int level) {
        switch (level) {
            case Warning: return Document::DiagnosticMessage::Warning;
            case Error:   return Document::DiagnosticMessage::Error;
            case Fatal:   return Document::DiagnosticMessage::Fatal;
            default:      return Document::DiagnosticMessage::Error;
        }
    }

    Document *doc;
    QList<Document::DiagnosticMessage> *messages;
};

} // anonymous namespace

Document::Document(const QString &fileName, Ptr parent)
  : _fileName(fileName), _parent(parent)
{
  _control = new Control();

  _control->setDiagnosticClient(new DocumentDiagnosticClient(this, &_diagnosticMessages));

  const QByteArray localFileName = fileName.toUtf8();
  StringLiteral *fileId = _control->findOrInsertFileName(localFileName.constData(),
                                                          localFileName.size());
  _translationUnit = new TranslationUnit(_control, fileId);
  _translationUnit->setQtMocRunEnabled(true);
  _translationUnit->setObjCEnabled(true);
  (void) _control->switchTranslationUnit(_translationUnit);
}

Document::~Document()
{
  delete _translationUnit;
  delete _control->diagnosticClient();
  delete _control;
}

QString Document::fileName() const
{
    return _fileName;
}

Document::Ptr Document::addIncludeFile(Ptr parent, const QString &fileName, unsigned line)
{
  Ptr includedDocument = create(parent, fileName);
  _includes.append(Include(includedDocument, line));
  return includedDocument;
}

void Document::appendMacro(const Macro &macro)
{
    _definedMacros.append(macro);
}

void Document::addMacroUse(const Macro &macro, unsigned offset, unsigned length,
                           const QVector<MacroArgumentReference> &actuals)
{
    MacroUse use(macro, offset, offset + length);

    foreach (const MacroArgumentReference &actual, actuals) {
        const Block arg(actual.position(), actual.position() + actual.length());

        use.addArgument(arg);
    }

    _macroUses.append(use);
}

void Document::check(Scope *globals) const
{
  foreach(Include inc, _includes)
    inc.document()->check(globals);

  if (!_translationUnit->isParsed() || !_translationUnit->ast())
    return;

  Semantic semantic(_control);
  if (TranslationUnitAST *ast = _translationUnit->ast()->asTranslationUnit())
  {
    for (DeclarationAST *decl = ast->declarations; decl; decl = decl->next)
    {
      semantic.check(decl, globals);
    }
  }
}

Document::Ptr Document::create(Ptr parent, const QString &fileName)
{
    Document::Ptr doc(new Document(fileName, parent));
    return doc;
}

bool Document::parse()
{
  return _translationUnit->parse(TranslationUnit::ParseTranlationUnit);
}

void Document::release()
{
  foreach(Include inc, _includes)
  {
    inc.document()->release();
    inc.document().clear();
  }

  _parent.clear();
}

void Document::setSource(const QByteArray &source)
{
   _source = source;
   _translationUnit->setSource(_source.constBegin(), _source.size());
}

void Document::startSkippingBlocks(unsigned start)
{
    _skippedBlocks.append(Block(start, 0));
}

void Document::stopSkippingBlocks(unsigned stop)
{
    unsigned start = _skippedBlocks.back().begin();
    if (start > stop)
        _skippedBlocks.removeLast(); // Ignore this block, it's invalid.
    else
        _skippedBlocks.back() = Block(start, stop);
}
