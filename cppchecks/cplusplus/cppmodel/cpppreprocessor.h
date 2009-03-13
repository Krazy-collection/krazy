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
#ifndef CPLUSPLUS_CPPMODEL_PREPROCESSOR_H
#define CPLUSPLUS_CPPMODEL_PREPROCESSOR_H

#include <QtCore/QSet>
#include <QtCore/QStringList>

#include <preprocessor/pp-engine.h>
#include <preprocessor/PreprocessorClient.h>
#include <preprocessor/PreprocessorEnvironment.h>

#include "document.h"

namespace CPlusPlus {

namespace CppModel {

class CPLUSPLUS_MODEL_EXPORT CppPreprocessor : public Client
{
  public: // Functions
    CppPreprocessor();

    /**
     * Sets the include paths where the preprocessor should look for global
     * header files. (e.g. for headers included like: #include <header.h>). The
     * first path in the list will be the first location where the preprocessor
     * will look for included global headers.
     *
     * @param includePaths The paths where the preprocessor should look for 
     *                     global header files.
     */
    void setGlobalIncludePaths(QStringList const &includePaths);

    /**
     * Sets the local include paths where the preprocessor should look for local
     * header files. (e.g. for headers included like: #include "header.h"). The
     * first path in the list will be the first location where the preprocessor
     * will look for included local headers.
     *
     * @param includePaths The paths where the preprocessor should look for
     *                     local header files.
     */
    void setLocalIncludePaths(QStringList const &includePaths);

    /**
     * Preprocesses the given file and creates Document objects for each inlcuded
     * file that is encountered during preprocessing.
     * 
     * @param fileName The initial file which must be preprocessed.
     * @return A Document object containing preprocessed code and pointers to
     *         included documents.
     */
    Document::Ptr operator()(QString &fileName);

    /**
     * Preprocesses the given file and returns the preprocessed source. This
     * method will <em>not</em> process eventually included documents. The
     * @param fileName must be the name of an existing file.
     * 
     * @param fileName The initial file which must be preprocessed.
     * @return A Document object containing preprocessed code and pointers to
     *         included documents.
     */
    QByteArray operator()(QString const &fileName);

  protected: // Enum
    enum Mode
    {
      Recursive,
      NonRecursive
    };

  protected: // Functions
    bool includeFile(QString const &absoluteFilePath, QByteArray *result);

    QByteArray tryIncludeFile(QString &fileName, IncludeType type, QString &msg);

    virtual void macroAdded(const Macro &macro);

    virtual void sourceNeeded(QString &fileName, IncludeType type, unsigned line);

    QByteArray sourceNeeded(QString const &fileName, unsigned line);

    virtual void startExpandingMacro(unsigned offset,
                                     Macro const &macro,
                                     QByteArray const &originalText,
                                     QVector<MacroArgumentReference> const &actuals);

    virtual void startSkippingBlocks(unsigned offset);

    virtual void stopExpandingMacro(unsigned offset, const Macro &macro);

    virtual void stopSkippingBlocks(unsigned offset);

    Document::Ptr switchDocument(Document::Ptr doc);

  private: // Members
    Document::Ptr m_currentDoc;
    Environment   m_env;
    QStringList   m_globalIncludePaths;
    QSet<QString> m_included;
    QStringList   m_localIncludePaths;
    Mode          m_mode;
    Preprocessor  m_proc;
    Document::Ptr m_rootDoc;
  };

} // namespace CppModel

} // namespace CPlusPlus

#endif
