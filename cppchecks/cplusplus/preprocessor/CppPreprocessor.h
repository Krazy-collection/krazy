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
    //CppPreprocessor(QPointer<CppModelManager> modelManager);

    void setWorkingCopy(const QMap<QString, QByteArray> &workingCopy);
    void setIncludePaths(const QStringList &includePaths);
    void setFrameworkPaths(const QStringList &frameworkPaths);
    void setProjectFiles(const QStringList &files);
    void run(QString &fileName);
    void operator()(QString &fileName);

protected:
    CPlusPlus::Document::Ptr switchDocument(CPlusPlus::Document::Ptr doc);

    bool includeFile(const QString &absoluteFilePath, QByteArray *result);
    QByteArray tryIncludeFile(QString &fileName, CPlusPlus::Client::IncludeType type);

    void mergeEnvironment(CPlusPlus::Document::Ptr doc);
    void mergeEnvironment(CPlusPlus::Document::Ptr doc, QSet<QString> *processed);

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
    //QPointer<CppModelManager> m_modelManager;
    Snapshot m_snapshot;
    CPlusPlus::Environment env;
    CPlusPlus::Preprocessor m_proc;
    QStringList m_includePaths;
    QStringList m_systemIncludePaths;
    QMap<QString, QByteArray> m_workingCopy;
    QStringList m_projectFiles;
    QStringList m_frameworkPaths;
    QSet<QString> m_included;
    CPlusPlus::Document::Ptr m_currentDoc;
};

} // namespace CPlusPlus

#endif
