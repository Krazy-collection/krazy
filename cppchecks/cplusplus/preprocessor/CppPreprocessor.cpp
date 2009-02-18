#include "CppPreprocessor.h"

#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QtDebug>
#include <QtCore/QTextStream>

#include <TranslationUnit.h>

using namespace CPlusPlus;

static const char pp_configuration_file[] = "<configuration>";

CppPreprocessor::CppPreprocessor(/*QPointer<CppModelManager> modelManager*/)
  : m_proc(this, env)
{
}

void CppPreprocessor::setWorkingCopy(const QMap<QString, QByteArray> &workingCopy)
{ m_workingCopy = workingCopy; }

void CppPreprocessor::setIncludePaths(const QStringList &includePaths)
{ m_includePaths = includePaths; }

void CppPreprocessor::setFrameworkPaths(const QStringList &frameworkPaths)
{ m_frameworkPaths = frameworkPaths; }

void CppPreprocessor::setProjectFiles(const QStringList &files)
{ m_projectFiles = files; }

void CppPreprocessor::run(QString &fileName)
{ sourceNeeded(fileName, IncludeGlobal, /*line = */ 0); }

void CppPreprocessor::operator()(QString &fileName)
{ run(fileName); }

bool CppPreprocessor::includeFile(const QString &absoluteFilePath, QByteArray *result)
{
    qDebug() << "CppPreprocessor::includeFile() " << absoluteFilePath;
    if (absoluteFilePath.isEmpty() || m_included.contains(absoluteFilePath)) {
        return true;
    }

    if (m_workingCopy.contains(absoluteFilePath)) {
        m_included.insert(absoluteFilePath);
        *result = m_workingCopy.value(absoluteFilePath);
        return true;
    }

    QFileInfo fileInfo(absoluteFilePath);
    if (! fileInfo.isFile())
        return false;

    QFile file(absoluteFilePath);
    if (file.open(QFile::ReadOnly)) {
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
    qDebug() << "CppPreprocessor::tryIncludeFile() " << fileName;
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

    // look in the system include paths
    foreach (const QString &includePath, m_systemIncludePaths) {
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

    int index = fileName.indexOf(QLatin1Char('/'));
    if (index != -1) {
        QString frameworkName = fileName.left(index);
        QString name = fileName.mid(index + 1);

        foreach (const QString &frameworkPath, m_frameworkPaths) {
            QString path = frameworkPath;
            path += QLatin1Char('/');
            path += frameworkName;
            path += QLatin1String(".framework/Headers/");
            path += name;
            QByteArray contents;
            if (includeFile(path, &contents)) {
                fileName = path;
                return contents;
            }
        }
    }

    QString path = fileName;
    if (path.at(0) != QLatin1Char('/'))
        path.prepend(QLatin1Char('/'));

    foreach (const QString &projectFile, m_projectFiles) {
        if (projectFile.endsWith(path)) {
            fileName = projectFile;
            QByteArray contents;
            includeFile(fileName, &contents);
            return contents;
        }
    }

    qDebug() << "**** file" << fileName << "not found!";
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

void CppPreprocessor::mergeEnvironment(Document::Ptr doc)
{
    QSet<QString> processed;
    mergeEnvironment(doc, &processed);
}

void CppPreprocessor::mergeEnvironment(Document::Ptr doc, QSet<QString> *processed)
{
    if (! doc)
        return;

    const QString fn = doc->fileName();

    if (processed->contains(fn))
        return;

    processed->insert(fn);

    foreach (QString includedFile, doc->includedFiles()) {
        mergeEnvironment(m_snapshot.value(includedFile), processed);
    }

    foreach (const Macro macro, doc->definedMacros()) {
        env.bind(macro);
    }
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
    qDebug() << "CppPreprocessor::sourceNeeded() " << fileName;
    if (fileName.isEmpty())
        return;

    QByteArray contents = tryIncludeFile(fileName, type);

    if (m_currentDoc) {
        m_currentDoc->addIncludeFile(fileName, line);
        if (contents.isEmpty() && ! QFileInfo(fileName).isAbsolute()) {
            QString msg;
            msg += fileName;
            msg += QLatin1String(": No such file or directory");
            Document::DiagnosticMessage d(Document::DiagnosticMessage::Warning,
                                          m_currentDoc->fileName(),
                                          env.currentLine, /*column = */ 0,
                                          msg);
            m_currentDoc->addDiagnosticMessage(d);
            //qWarning() << "file not found:" << fileName << m_currentDoc->fileName() << env.current_line;
        }
    }

    if (! contents.isEmpty()) {
        Document::Ptr cachedDoc = m_snapshot.value(fileName);
        if (cachedDoc && m_currentDoc) {
            mergeEnvironment(cachedDoc);
        } else {
            Document::Ptr previousDoc = switchDocument(Document::create(fileName));

            const QByteArray previousFile = env.currentFile;
            const unsigned previousLine = env.currentLine;

            env.currentFile = QByteArray(m_currentDoc->translationUnit()->fileName(),
                                         m_currentDoc->translationUnit()->fileNameLength());

            QByteArray preprocessedCode;
            m_proc(contents, &preprocessedCode);
            //qDebug() << preprocessedCode;

            env.currentFile = previousFile;
            env.currentLine = previousLine;

            m_currentDoc->setSource(preprocessedCode);
            m_currentDoc->parse();
            m_currentDoc->check();
            m_currentDoc->releaseTranslationUnit(); // release the AST and the token stream.

//            if (m_modelManager)
//              m_modelManager->emitDocumentUpdated(m_currentDoc);
            (void) switchDocument(previousDoc);
        }
    }
}

Document::Ptr CppPreprocessor::switchDocument(Document::Ptr doc)
{
    Document::Ptr previousDoc = m_currentDoc;
    m_currentDoc = doc;
    return previousDoc;
}
