/*
    Copyright (C) 2009 Bertjan Broeksema <b.broeksema@kdemail.net>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "document.h"

#include <parser/AST.h>
#include <parser/Control.h>
#include <parser/DiagnosticClient.h>
#include <parser/Literals.h>
#include <parser/Scope.h>
#include <parser/Semantic.h>
#include <parser/Symbols.h>
#include <parser/TranslationUnit.h>

#include <preprocessor/PreprocessorClient.h>

#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QtDebug>

using namespace CPlusPlus;
using namespace CppModel;

namespace {

  class DocumentDiagnosticClient : public DiagnosticClient
  {
    enum { MAX_MESSAGE_COUNT = 10 };

    public:
      DocumentDiagnosticClient(Document *doc, QList<DiagnosticMessage> *messages)
        : doc(doc),
          messages(messages)
      {}

      virtual void report(int level,
                          StringLiteral *fileId,
                          unsigned line, unsigned column,
                          const char *format, va_list ap)
      {
        if (messages->count() == MAX_MESSAGE_COUNT)
          return;

        QString const fileName = QString::fromUtf8(fileId->chars(), fileId->size());

        if (fileName != doc->fileName())
          return;

        QString message;
        message.vsprintf(format, ap);

        DiagnosticMessage m(convertLevel(level), doc->fileName(),
                            line, column, message);
        messages->append(m);
      }
      
      static DiagnosticMessage::Level convertLevel(int level) 
      {
        switch (level) 
        {
          case Warning: return DiagnosticMessage::Warning;
          case Error:   return DiagnosticMessage::Error;
          case Fatal:   return DiagnosticMessage::Fatal;
          default:      return DiagnosticMessage::Error;
        }
      }

      Document *doc;
      QList<DiagnosticMessage> *messages;
  };

} // anonymous namespace

/// Implementation of Include

Document::Include::Include(Ptr const &document, Client::IncludeType type, unsigned line)
  : m_document(document), m_line(line), m_type(type)
{}

Document::Ptr Document::Include::document() const
{
  return m_document;
}

unsigned Document::Include::line() const
{
  return m_line;
}

Client::IncludeType Document::Include::type() const
{
  return m_type;
}

/// Implementation of Document

/// Document :: public functions

Document::~Document()
{
  delete _translationUnit;
  delete _control->diagnosticClient();
  delete _control;
}

void Document::check()
{
  /*
  foreach(Include inc, _includes)
    inc.document()->check(globals);

  if (!_translationUnit->ast())
    parse();

  Semantic semantic(_control);
  if (TranslationUnitAST *ast = _translationUnit->ast()->asTranslationUnit())
  {
    for (DeclarationAST *decl = ast->declarations; decl; decl = decl->next)
    {
      semantic.check(decl, globals);
    }
  }
  */
}

QList<Macro> Document::definedMacros() const
{
  return _definedMacros;
}

QList<DiagnosticMessage> Document::diagnosticMessages() const
{
  return _diagnosticMessages;
}

QString Document::fileName() const
{
  return _fileName;
}

QList<Document::Include> Document::includes() const
{
  return _includes;
}

QList<MacroUse> Document::macroUses() const
{
  return _macroUses;
}

QList<CharBlock> Document::skippedBlocks() const
{
  return _skippedBlocks;
}

/// Document :: Protected functions

/*

void Document::addDiagnosticMessage(DiagnosticMessage const &message)
{
  _diagnosticMessages.append(message);
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

Document::Ptr Document::create(Ptr parent, const QString &fileName)
{
    Document::Ptr doc(new Document(fileName, parent));
    return doc;
}

void Document::setSource(QByteArray const &source)
{
  // NOTE: We don't store the actual source at all in the document object. From
  //       the source we create directly an unannotated AST.
  _translationUnit->setSource(source.constBegin(), source.size());
  _translationUnit->tokenize();
  _translationUnit->parse(TranslationUnit::ParseTranlationUnit);
  _translationUnit->release();
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
*/

/// Document :: Private functions

Document::Document(const QString &fileName)
  : _fileName(fileName)
  , _control(new Control())
{
  _control->setDiagnosticClient(new DocumentDiagnosticClient(this, &_diagnosticMessages));

  const QByteArray localFileName = fileName.toUtf8();
  StringLiteral *fileId = _control->findOrInsertFileName(localFileName.constData(),
                                                          localFileName.size());
  _translationUnit = new TranslationUnit(_control, fileId);
  _translationUnit->setQtMocRunEnabled(true);
  _translationUnit->setObjCEnabled(true);

  (void) _control->switchTranslationUnit(_translationUnit);
}
