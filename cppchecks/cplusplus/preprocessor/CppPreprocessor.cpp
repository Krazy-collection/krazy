#include "CppPreprocessor.h"

#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>

#include <TranslationUnit.h>

using namespace CPlusPlus;

static const char pp_configuration_file[] = "<configuration>";

CppPreprocessor::CppPreprocessor()
  : m_proc(this, env)
{}

void CppPreprocessor::setIncludePaths(const QStringList &includePaths)
{ 
  m_includePaths = includePaths; 
}

CPlusPlus::Document::Ptr CppPreprocessor::run(QString &fileName)
{
  sourceNeeded(fileName, IncludeGlobal, /*line = */ 0);
  return m_rootDoc;
}

bool CppPreprocessor::includeFile(const QString &absoluteFilePath, QByteArray *result)
{
    if (absoluteFilePath.isEmpty() || m_included.contains(absoluteFilePath)) {
        return true;
    }

    QFileInfo fileInfo(absoluteFilePath);
    if (! fileInfo.isFile())
        return false;

    QFile file(absoluteFilePath);
    if (file.open(QFile::ReadOnly)) {
        // qDebug() << "CppPreprocessor::includeFile() including" << absoluteFilePath;
        m_included.insert(absoluteFilePath);
        QTextStream stream(&file);
        const QString contents = stream.readAll();
        *result = contents.toUtf8();
        file.close();
        return true;
    }

    return false;
}

QByteArray CppPreprocessor::tryIncludeFile(QString &fileName, IncludeType type)
{
    QFileInfo fileInfo(fileName);
    if (fileName == QLatin1String(pp_configuration_file) || fileInfo.isAbsolute()) {
        QByteArray contents;
        includeFile(fileName, &contents);
        return contents;
    }

    if (type == IncludeLocal && m_currentDoc) {
        QFileInfo currentFileInfo(m_currentDoc->fileName());
        QString path = currentFileInfo.absolutePath();
        path += QLatin1Char('/');
        path += fileName;
        path = QDir::cleanPath(path);
        QByteArray contents;
        if (includeFile(path, &contents)) {
            fileName = path;
            return contents;
        }
    }

    foreach (const QString &includePath, m_includePaths) {
        QString path = includePath;
        path += QLatin1Char('/');
        path += fileName;
        path = QDir::cleanPath(path);
        QByteArray contents;
        if (includeFile(path, &contents)) {
            fileName = path;
            return contents;
        }
    }

    // TODO: Maybe store a list of files that could not be found.
    // qDebug() << "**** file" << fileName << "not found!";
    return QByteArray();
}

void CppPreprocessor::macroAdded(const Macro &macro)
{
    if (! m_currentDoc)
        return;

    m_currentDoc->appendMacro(macro);
}

void CppPreprocessor::startExpandingMacro(unsigned offset,
                                          const Macro &macro,
                                          const QByteArray &originalText)
{
    if (! m_currentDoc)
        return;

    //qDebug() << "start expanding:" << macro.name << "text:" << originalText;
    m_currentDoc->addMacroUse(macro, offset, originalText.length());
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
    if (fileName.isEmpty())
        return;

    QByteArray contents = tryIncludeFile(fileName, type);
    Document::Ptr previousDoc;

    if (m_currentDoc) { // We're reading an #include of the current document.
        Document::Ptr includedDoc = m_currentDoc->addIncludeFile(m_currentDoc, fileName, line);
        previousDoc = switchDocument(includedDoc);

        if (contents.isEmpty() && !QFileInfo(fileName).isAbsolute()) {
            QString msg;
            msg += fileName;
            msg += QLatin1String(": No such file or directory");
            Document::DiagnosticMessage d(Document::DiagnosticMessage::Warning,
                                          m_currentDoc->fileName(),
                                          env.currentLine, /*column = */ 0,
                                          msg);
            previousDoc->addDiagnosticMessage(d);
        }
    } else { // We're reading the "root" file.
      m_rootDoc = Document::create(Document::Ptr(0), fileName);
      previousDoc = switchDocument(m_rootDoc);
    }

    if (!contents.isEmpty()) {
      // Ath this point m_currentDoc is the document that is include or the root
      // file if sourceNeeded is called for the first time.

        const QByteArray previousFile = env.currentFile;
        const unsigned previousLine = env.currentLine;

        env.currentFile = m_currentDoc->fileName().toUtf8();

        // Preprocess and parse the documents included in the current document.
        QByteArray preprocessedCode;
        m_proc(contents, &preprocessedCode);

        env.currentFile = previousFile;
        env.currentLine = previousLine;

        m_currentDoc->setSource(preprocessedCode);
    }

    (void) switchDocument(previousDoc); // We have switched, so switch back
}

Document::Ptr CppPreprocessor::switchDocument(Document::Ptr doc)
{
    Document::Ptr previousDoc = m_currentDoc;
    m_currentDoc = doc;
    return previousDoc;
}
