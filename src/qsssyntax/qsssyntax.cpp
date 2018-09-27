/*
 * Sanity check plugin for the Krazy project.
 * Copyright (C) 2014 by Allen Winter <winter@kde.org>
 * Copyright (C) 2014 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Sergio Martins <sergio.martins@kdab.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/*
 * Tests QML code for syntax errors
 *
 * Program options:
 *   --help:          print one-line help message and exit
 *   --version:       print one-line version information and exit
 *   --explain:       print an explanation with solving instructions
 *   --quiet:         suppress all output messages
 *   --verbose:       print the offending content
 *
 * Exits with status=0 if test condition is not present in the source;
 * else exits with the number of failures encountered.
 */

//TODO: is it possible to support Krazy in-code directives?
//TODO: is it possible to support verbose?

#include <QGuiApplication>
#include <QFile>
#include <QFileInfo>

#include <QtQml/private/qqmljsengine_p.h>
#include <QtQml/private/qqmljslexer_p.h>
#include <QtQml/private/qqmljsparser_p.h>

#if QT_VERSION >= 0x050300
# include <QtQml/private/qqmlirbuilder_p.h>
#else
# include <QtQml/private/qqmlscript_p.h>
#endif

#if QT_VERSION < 0x050500
static void remove_metadata(QString &code)
{
    // Removes .pragma library and such, otherwise we get a syntax error.
#if QT_VERSION >= 0x050400
        QmlIR::Document::removeScriptPragmas(/*by-ref*/code);
#elif QT_VERSION >= 0x050300
        QQmlJS::DiagnosticMessage metaDataError;
        QmlIR::Document irUnit(/**debugger=*/false);
        irUnit.extractScriptMetaData(code, &metaDataError);
#else
        QQmlError metaDataError;
        QQmlScript::Parser::extractMetaData(code, &metaDataError);
#endif
}
#endif

static QString lineList(const QList<int> &issues)
{
  QString list;

  int size = issues.size();
  if (size < 0) {
    return list;
  }

  list = QString("line#") + QString::number(issues[0]);

  for (int i=1; i<size; ++i) {
    list = QString(",") + QString::number(issues[i]);
  }

  return list;
}

static QMap<QString, QList<int> > lint_file(const QString &filename, bool silent)
{
    Q_UNUSED(silent);
    QMap<QString, QList<int> > issues;

    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        return issues;
    }

    QString code = QString::fromUtf8(file.readAll());
    file.close();

    QQmlJS::Engine engine;
    QQmlJS::Lexer lexer(&engine);

    QFileInfo info(filename);
    bool isJavaScript = info.suffix().toLower() == QLatin1String("js");
#if QT_VERSION < 0x050500
    if (isJavaScript) {
        remove_metadata(/*by-ref*/code);
    }
    lexer.setCode(code, /*line = */1, true);
#else
    lexer.setCode(code, /*line = */ 1, /*qmlMode=*/ !isJavaScript);
#endif
    QQmlJS::Parser parser(&engine);

    bool success = isJavaScript ? parser.parseProgram() : parser.parse();

    if (!success) {
        foreach (const QQmlJS::DiagnosticMessage &m, parser.diagnosticMessages()) {
            if (issues.contains(m.message)) {
                if (!issues.value(m.message).contains(m.loc.startLine)) {
                  issues[m.message] << m.loc.startLine;
                }
            } else {
                QList<int> lines;
                lines[0] = m.loc.startLine;
                issues.insert(m.message, lines);
            }
        }
    }

    return issues;
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stderr, "usage: %s <options> <source-file>\n", argv[0]);
        return 0;
    }

    bool verbose = false;
    bool quiet = false;

    QString fileName;

    /* I *did* make it myself, how did you know!? */
    for (--argc, ++argv; argc; --argc, ++argv) {
        if (!strcmp(*argv, "--verbose")) {
            verbose = true;
        } else if (!strcmp(*argv,"--quiet")) {
            quiet = true;
        } else if (!strcmp(*argv,"--version")) {
            printf("qsssyntax, version 0.1\n");
            return 0;
        } else if (!strcmp(*argv,"--help")) {
            printf("Check for syntax errors in Qt stylesheets\n");
            return 0;
        } else if (!strcmp(*argv, "--explain")) {
            printf("This check shows syntax errors in your Qt stylesheets. Fix those errors.\n");
            return 0;
        } else {
            fileName = QString::fromLatin1(*argv);
            break;
        }
    }

    if (fileName.isEmpty()) {
        return 1;
    }

    if (!fileName.toLower().endsWith(".qss")) {
        if (!quiet) {
            printf("okay\n");
        }
        return 0;
    }

    bool silent = true;
    if (!quiet && verbose) {
        silent = false;
    }

    QMap<QString, QList<int> > issues;
    issues = lint_file(fileName, silent);
    int nastyCount = issues.count();

    if (nastyCount == 0) {
        if (!quiet) {
            printf("okay\n");
        }
        return 0;
    }

    int total = 0;
    QMap<QString, QList<int> >::const_iterator i = issues.constBegin();
    while (i != issues.constEnd()) {
      printf("%s: %s %s\n",
             qPrintable(fileName),
             qPrintable(i.key()),
             qPrintable(lineList(i.value())));
      total += (i.value().count() + 1);
      ++i;
    }
    printf("ISSUES=%d\n", total);
    return total;
}
/* kate: space-indent on; indent-width 4; */
