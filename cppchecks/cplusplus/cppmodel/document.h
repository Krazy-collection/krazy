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
#ifndef CPLUSPLUS_CPPMODEL_DOCUMENT_H
#define CPLUSPLUS_CPPMODEL_DOCUMENT_H

// Needed for Namespace, Should be removed by moving members to private data class.
#include <parser/Symbols.h>
#include <preprocessor/Macro.h>
#include <preprocessor/PreprocessorClient.h>

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "cppmodel-config.h"
#include "diagnosticmessage.h"
#include "macrouse.h"

namespace CPlusPlus {

  class Control;
  class MacroArgumentReference;
  class TranslationUnit;

namespace CppModel {

  class CPLUSPLUS_MODEL_EXPORT Document
  {
    public:
      typedef QSharedPointer<Document> Ptr;

      class CPLUSPLUS_MODEL_EXPORT Include 
      {
        public:
          /**
          * Creates a new Include object, where @param document is the included
          * document and @param line the line at which the included document was
          * included.
          */
          Include(Ptr const &document, Client::IncludeType type, unsigned line);

          Ptr document() const;

          unsigned line() const;

          Client::IncludeType type() const;

        private:
          Ptr                 m_document; /// Pointer to the included document.
          unsigned            m_line;     /// The line at which this document was included.
          Client::IncludeType m_type;     /// Global or Local include.
      };

    public:
      ~Document();

      QString absoluteFileName() const;

      QList<Macro> definedMacros() const;

      QList<DiagnosticMessage> diagnosticMessages() const;

      QString fileName() const;

      Namespace * const globalNamespace() const;

      QList<Include> includes() const;

      QList<MacroUse> macroUses() const;

      QList<CharBlock> skippedBlocks() const;

      TranslationUnit * translationUnit() const;

    protected: // Functions
      friend class CppPreprocessor; /// Only a preprocessor can modify a document.

      void addDiagnosticMessage(DiagnosticMessage const &message);

      void addIncludeFile(Ptr const &includedDocument, Client::IncludeType type, unsigned line);

      void addMacroUse(Macro const &macro, unsigned offset, unsigned length,
                       QVector<MacroArgumentReference> const &range);

      void appendMacro(Macro const &macro);

      /**
       * This will run the semantic pass over the AST and the AST of all included
       * documents. If the AST is not created yet, the document will be parsed
       * first. The semantic symbols will be added to the @param globals Scope
       * object.
       */
      void check(QSharedPointer<Namespace> globalNamespace = QSharedPointer<Namespace>());

      static Ptr create(QString const &fileName);

      void setPath(QString const &path);

      void setSource(QByteArray const &source);

      void startSkippingBlocks(unsigned offset);

      void stopSkippingBlocks(unsigned offset);

    private: // Functions
      Document(const Document &other);

      void operator=(const Document &other);

      Document(const QString &fileName);

    private: // Members
      // TODO: Move the members to a private data class.
      Control*                  m_control;
      QString                   m_fileName;
      QList<Macro>              m_definedMacros;
      QList<DiagnosticMessage>  m_diagnosticMessages;
      QSharedPointer<Namespace> m_globalNamespace;
      QList<Include>            m_includes;
      QList<MacroUse>           m_macroUses;
      QString                   m_path;
      QList<CharBlock>          m_skippedBlocks;
      QByteArray                m_source;
      TranslationUnit          *m_translationUnit;
  };

} // namespace CppModel

} // namespace CPlusPlus

Q_DECLARE_METATYPE(CPlusPlus::CppModel::Document::Ptr);

#endif // CPLUSPLUS_CPPMODEL_DOCUMENT_H
