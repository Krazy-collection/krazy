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
#ifndef CPLUSPLUS_PREPROCESSOR_H
#define CPLUSPLUS_PREPROCESSOR_H

#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QStringList>

#include "CppDocument.h"
#include "pp-engine.h"
#include "PreprocessorClient.h"
#include "PreprocessorEnvironment.h"

namespace CPlusPlus {

class CppPreprocessor: public CPlusPlus::Client
{
public:
    CppPreprocessor();

    /**
     * Preprocesses the given file and creates Document objects for each inlcuded
     * file that is encountered during preprocessing.
     * 
     * @param fileName The initial file which must be preprocessed.
     * @return A list of documents containing preprocessed code.
     */
    QList<CPlusPlus::Document::Ptr> run(QString &fileName);

    /**
     * Set the include paths where the preprocessor should look for header files.
     *
     * @param includePaths The paths where the preprocessor should look for 
     *                     header files.
     */
    void setIncludePaths(const QStringList &includePaths);

protected:
    CPlusPlus::Document::Ptr switchDocument(CPlusPlus::Document::Ptr doc);

    bool includeFile(const QString &absoluteFilePath, QByteArray *result);
    QByteArray tryIncludeFile(QString &fileName, CPlusPlus::Client::IncludeType type);

    virtual void macroAdded(const Macro &macro);
    virtual void startExpandingMacro(unsigned offset,
                                     const Macro &macro,
                                     const QByteArray &originalText);
    virtual void stopExpandingMacro(unsigned offset, const Macro &macro);
    virtual void startSkippingBlocks(unsigned offset);
    virtual void stopSkippingBlocks(unsigned offset);
    virtual void sourceNeeded(QString &fileName, IncludeType type,
                              unsigned line);

private:
    CPlusPlus::Environment env;
    CPlusPlus::Preprocessor m_proc;
    QStringList m_includePaths;
    QSet<QString> m_included;
    CPlusPlus::Document::Ptr m_currentDoc;
    QList<CPlusPlus::Document::Ptr> m_documents;
};

} // namespace CPlusPlus

#endif
