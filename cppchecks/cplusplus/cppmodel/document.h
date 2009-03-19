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

  class NamespaceBinding;

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

      /**
       * Returns the NamespaceBinding object for the global namespace. This
       * contains binded namespace in opposite of namespace forrests.
       *
       * @see Namespace * const globalNamespace() const
       * @see void bind()
       */
      NamespaceBinding * const boundGlobalNamespace() const;

      QList<Macro> definedMacros() const;

      QList<DiagnosticMessage> diagnosticMessages() const;

      QString fileName() const;

      /**
       * Returns the Namespace object for the global namespace.
       *
       * @see NamespaceBinding * const boundGlobalNamespace() const
       * @see void bind()
       */
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
       * The semantic pass creates forrests of namespace trees. I.e. everytime
       * it finds a namespace definition it does not check if the namespace
       * already is created but just creates a new Namespace object with the
       * same name and adds it to the scope where the namespace was defined. In
       * this model we're more interested in a tree where all namespaces with
       * the same name in the same scope are handled as the same logical
       * namespace. This method does the actual binding and should be called
       * after the check method.
       */
      void bind();

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
      NamespaceBinding         *m_binding;
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
