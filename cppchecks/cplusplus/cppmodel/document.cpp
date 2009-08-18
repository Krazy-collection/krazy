/*
   This file or code within this file was originally part of Qt Creator.

   Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
   Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
   Copyright (c) 2009 Bertjan Broeksema <b.broeksema@kdemail.net>

   GNU Lesser General Public License Usage

   This file may be used under the terms of the GNU Lesser
   General Public License version 2.1 as published by the Free Software
   Foundation and appearing in the file LICENSE.LGPL included in the
   packaging of this file.  Please review the following information to
   ensure the GNU Lesser General Public License version 2.1 requirements
   will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
*/
#include "document.h"

#include <parser/AST.h>
#include <parser/Control.h>
#include <parser/DiagnosticClient.h>
#include <parser/Literals.h>
#include <parser/Scope.h>
#include <parser/Semantic.h>
#include <parser/TranslationUnit.h>

#include <preprocessor/PreprocessorClient.h>

#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QtDebug>

#include "binder.h"
#include "namespacebinding.h"

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
  : m_document(document)
  , m_line(line)
  , m_type(type)
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
  delete m_translationUnit;
  delete m_control->diagnosticClient();
  delete m_control;
  delete m_binding;
}

QString Document::absoluteFileName() const
{
  if (m_path.isEmpty() || m_path.endsWith(QDir::separator()))
    return m_path + m_fileName;
  else
    return m_path + QDir::separator() + m_fileName;
}

NamespaceBinding * const Document::boundGlobalNamespace() const
{
  return m_binding;
}

QList<Macro> Document::definedMacros() const
{
  return m_definedMacros;
}

QList<DiagnosticMessage> Document::diagnosticMessages() const
{
  return m_diagnosticMessages;
}

QString Document::fileName() const
{
  return m_fileName;
}

Namespace * const Document::globalNamespace() const
{
  return m_globalNamespace.data();
}

QList<Document::Include> Document::includes() const
{
  return m_includes;
}

QList<MacroUse> Document::macroUses() const
{
  return m_macroUses;
}

QList<CharBlock> Document::skippedBlocks() const
{
  return m_skippedBlocks;
}

TranslationUnit * Document::translationUnit() const
{
  return m_translationUnit;
}

/// Document :: Protected functions

void Document::addDiagnosticMessage(DiagnosticMessage const &message)
{
  m_diagnosticMessages.append(message);
}

void Document::addIncludeFile(Ptr const &includedDocument
                            , Client::IncludeType type
                            , unsigned line)
{
  m_includes.append(Include(includedDocument, type, line));
}

void Document::addMacroUse(Macro const &macro
                         , unsigned offset
                         , unsigned length
                         , QVector<MacroArgumentReference> const &actuals)
{
  MacroUse use(macro, offset, offset + length);

  foreach (const MacroArgumentReference &actual, actuals) {
    const CharBlock arg(actual.position(), actual.position() + actual.length());

    use.addArgument(arg);
  }

  m_macroUses.append(use);
}

void Document::appendMacro(Macro const &macro)
{
  m_definedMacros.append(macro);
}

void Document::bind()
{
  Binder bind;
  m_binding = bind(m_translationUnit, m_globalNamespace.data());
}

static void setToZero(Namespace *obj)
{
  obj = 0;
}

void Document::check(QSharedPointer<Namespace> globalNamespace)
{
  if (globalNamespace)
    m_globalNamespace = globalNamespace;
  else
    // The namespace is deleted when the Control object is deleted so the shared
    // pointer should not delete the object when the last goes out of scope.
    m_globalNamespace = QSharedPointer<Namespace>(m_control->newNamespace(0), setToZero);

  // Check the included documents.
  foreach(Include inc, m_includes)
    inc.document()->check(m_globalNamespace);

  if (! m_translationUnit->ast())
    return; // nothing to do.

  // Now check this document.
  Semantic semantic(m_control);
  Scope *globals = m_globalNamespace->members();

  if (TranslationUnitAST *ast = m_translationUnit->ast()->asTranslationUnit()){
    for (DeclarationListAST *decl = ast->declarations; decl; decl = decl->next) {
      semantic.check(decl->declaration, globals);
    }
  } else if (ExpressionAST *ast = m_translationUnit->ast()->asExpression()) {
    semantic.check(ast, globals);
  }
}

Document::Ptr Document::create(QString const &fileName)
{
  Document::Ptr doc(new Document(fileName));
  return doc;
}

void Document::setPath(QString const &path)
{
  m_path = path;
}

void Document::setSource(QByteArray const &source)
{
  // NOTE: We don't store the actual source at all in the document object. From
  //       the source we create directly an unannotated AST.
  m_translationUnit->setSource(source.constBegin(), source.size());
  m_translationUnit->tokenize();
  m_translationUnit->parse(TranslationUnit::ParseTranlationUnit);
  //m_translationUnit->release();
}

void Document::startSkippingBlocks(unsigned start)
{
  m_skippedBlocks.append(CharBlock(start, 0));
}

void Document::stopSkippingBlocks(unsigned stop)
{
  unsigned start = m_skippedBlocks.back().begin();
  if (start > stop)
    m_skippedBlocks.removeLast(); // Ignore this block, it's invalid.
  else
    m_skippedBlocks.back() = CharBlock(start, stop);
}

/// Document :: Private functions

Document::Document(const QString &fileName)
  : m_binding(0)
  , m_control(new Control())
  , m_fileName(fileName)
{
  m_control->setDiagnosticClient(new DocumentDiagnosticClient(this, &m_diagnosticMessages));

  const QByteArray localFileName = fileName.toUtf8();
  StringLiteral *fileId = m_control->findOrInsertStringLiteral(localFileName.constData(),
                                                               localFileName.size());
  m_translationUnit = new TranslationUnit(m_control, fileId);
  m_translationUnit->setQtMocRunEnabled(true);
  m_translationUnit->setObjCEnabled(true);

  (void) m_control->switchTranslationUnit(m_translationUnit);
}
