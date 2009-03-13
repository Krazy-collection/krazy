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
#include "cpppreprocessor.h"

#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>

#include <parser/TranslationUnit.h>

using namespace CPlusPlus;
using namespace CppModel;

// TODO: Figure out what this servers for.
static const char pp_configuration_file[] = "<configuration>";

/// CppPreprocessor :: public functions

CppPreprocessor::CppPreprocessor()
  : m_proc(this, &m_env)
{}

void CppPreprocessor::setGlobalIncludePaths(QStringList const &includePaths)
{
  m_globalIncludePaths = includePaths; 
}

void CppPreprocessor::setLocalIncludePaths(QStringList const &includePaths)
{
  m_localIncludePaths = includePaths; 
}

Document::Ptr CppPreprocessor::operator()(QString &fileName)
{
  m_mode = Recursive;
  sourceNeeded(fileName, IncludeGlobal, /*line = */ 0);
  return m_rootDoc;
}

QByteArray CppPreprocessor::operator()(QString const &fileName)
{
  m_mode = NonRecursive;
  return sourceNeeded(fileName, /*line = */ 0);
}

/// CppPreprocessor :: protected functions

bool CppPreprocessor::includeFile(QString const &absoluteFilePath, QByteArray *result)
{
  if (absoluteFilePath.isEmpty() || m_included.contains(absoluteFilePath))
    return true;

  QFileInfo fileInfo(absoluteFilePath);
  if (! fileInfo.isFile())
    return false;

  QFile file(absoluteFilePath);
  if (file.open(QFile::ReadOnly)) {
    // qDebug() << "CppPreprocessor::includeFile() including" << absoluteFilePath;
    m_included.insert(absoluteFilePath);
    QTextStream stream(&file);
    QString const contents = stream.readAll();
    *result = contents.toUtf8();
    file.close();
    return true;
  }

  return false;
}

QByteArray CppPreprocessor::tryIncludeFile(QString &fileName, IncludeType type, QString &msg)
{
  QFileInfo fileInfo(fileName);
  if (fileName == QLatin1String(pp_configuration_file) || fileInfo.isAbsolute())
  {
    QByteArray contents;
    includeFile(fileName, &contents);
    return contents;
  }

  if (type == IncludeLocal && m_currentDoc)
  {
    // First try the same directory as the document that requests the include.
    QFileInfo currentFileInfo(m_currentDoc->fileName());
    QString path = currentFileInfo.absolutePath();
    path += QLatin1Char('/');
    path += fileName;
    path = QDir::cleanPath(path);
    QByteArray contents;
    if (includeFile(path, &contents))
    {
      fileName = path;
      return contents;
    }
    // Otherwhise try the local include paths.
    else
    {
      foreach (QString const &includePath, m_localIncludePaths)
      {
        QString path = includePath;
        path += QLatin1Char('/');
        path += fileName;
        path = QDir::cleanPath(path);
        QByteArray contents;
        if (includeFile(path, &contents))
        {
          fileName = path;
          return contents;
        }
      }
    }
  }

  foreach (QString const &includePath, m_globalIncludePaths)
  {
    QString path = includePath;
    path += QLatin1Char('/');
    path += fileName;
    path = QDir::cleanPath(path);
    QByteArray contents;
    if (includeFile(path, &contents))
    {
      fileName = path;
      // TODO: Store a list of files that could not be found.
      if (type == IncludeGlobal)
        msg = fileName;
        msg += QLatin1String(": is included as a local include "
                             "but was only found in the global include path.");
      return contents;
    }
  }

  if (type == IncludeLocal)
    msg = '"' + fileName + '"' + QLatin1String(": No such file.");
  else
    msg = '<' + fileName + '>' + QLatin1String(": No such file.");
  return QByteArray();
}

void CppPreprocessor::macroAdded(Macro const &macro)
{
  if (!m_currentDoc)
    return;

  m_currentDoc->appendMacro(macro);
}

void CppPreprocessor::startExpandingMacro(
                                 unsigned offset,
                                 const Macro &macro,
                                 const QByteArray &originalText,
                                 const QVector<MacroArgumentReference> &actuals)
{
  if (! m_currentDoc)
      return;

  //qDebug() << "start expanding:" << macro.name << "text:" << originalText;
  m_currentDoc->addMacroUse(macro, offset, originalText.length(), actuals);
}

void CppPreprocessor::stopExpandingMacro(unsigned, const Macro &)
{
  if (! m_currentDoc)
      return;

  //qDebug() << "stop expanding:" << macro.name;
}

void CppPreprocessor::startSkippingBlocks(unsigned offset)
{
  //qDebug() << "start skipping blocks:" << offset;
  if (m_currentDoc)
    m_currentDoc->startSkippingBlocks(offset);
}

void CppPreprocessor::stopSkippingBlocks(unsigned offset)
{
  //qDebug() << "stop skipping blocks:" << offset;
  if (m_currentDoc)
    m_currentDoc->stopSkippingBlocks(offset);
}

void CppPreprocessor::sourceNeeded(QString &fileName, IncludeType type,
                                   unsigned line)
{
  if (fileName.isEmpty() || m_mode == NonRecursive)
      return; // Nothing todo.

  Document::Ptr previousDoc;
  if (m_currentDoc) // We're reading an #include of the current document.
  {
    Document::Ptr includedDoc = Document::create(fileName);
    m_currentDoc->addIncludeFile(includedDoc, type, line);
    previousDoc = switchDocument(includedDoc);
  }
  else // We're reading the "root" file.
  {
    m_rootDoc = Document::create(fileName);
    previousDoc = switchDocument(m_rootDoc);
  }

  QString msg;
  QByteArray contents = tryIncludeFile(fileName, type, msg);

  if (contents.isEmpty() && !QFileInfo(fileName).isAbsolute())
  {
    DiagnosticMessage d(DiagnosticMessage::Warning,
                        m_currentDoc->fileName(),
                        m_env.currentLine, /*column = */ 0,
                        msg);
    if (previousDoc)
      previousDoc->addDiagnosticMessage(d); // The included doc does not exist.
    else
      m_rootDoc->addDiagnosticMessage(d); // The root document does not exisit.
  }

  if (!contents.isEmpty())
  {
    // Ath this point m_currentDoc is the document that is include or the root
    // file if sourceNeeded is called for the first time.
    QByteArray const previousFile = m_env.currentFile;
    unsigned const previousLine = m_env.currentLine;

    m_env.currentFile = m_currentDoc->fileName().toUtf8();

    // Preprocess and parse the documents included in the current document.
    QByteArray preprocessedCode = m_proc(fileName.toUtf8(), contents);

    m_env.currentFile = previousFile;
    m_env.currentLine = previousLine;

    m_currentDoc->setPath(fileName.remove(m_currentDoc->fileName()));
    m_currentDoc->setSource(preprocessedCode);
  }

  (void) switchDocument(previousDoc); // We have switched, so switch back
}

QByteArray CppPreprocessor::sourceNeeded(QString const &fileName, unsigned line)
{
  QFile file(fileName);

  if (!file.exists())
    return QByteArray();

  if (file.open(QFile::ReadOnly)) 
  {
    QTextStream stream(&file);
    QByteArray contents = stream.readAll().toUtf8();
    file.close();
    return m_proc(fileName.toUtf8(), contents);
  }

  return QByteArray();
}

Document::Ptr CppPreprocessor::switchDocument(Document::Ptr doc)
{
  Document::Ptr previousDoc = m_currentDoc;
  m_currentDoc = doc;
  return previousDoc;
}
