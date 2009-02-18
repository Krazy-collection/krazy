/*
    Copyright (C) 2008 Bertjan Broeksema <b.broeksema@kdemail.net>

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
#include "checkengine.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QtDebug>
#include <QtCore/QUrl>

#include "AST.h"
#include "CppPreprocessor.h"
#include "Control.h"
#include "Scope.h"
#include "Semantic.h"
#include "TranslationUnit.h"

using namespace CPlusPlus;

CheckEngine::CheckEngine(Analyzer *analyzer) : m_analyzer(analyzer)
{}

CheckEngine::~CheckEngine()
{
  delete m_analyzer;
}

void CheckEngine::process(QUrl const &file)
{
  // Preproces the file
//   QProcess preprocessor;
//   preprocessor.start("cpp", QStringList() << file.path());
// 
//   if (!preprocessor.waitForFinished())
//   {
//     qDebug() << "Preprocessing failed:" << preprocessor.errorString();
//     return;
//   }

  // TODO Make this configurable or read from commandline.
  QStringList includePaths;
  includePaths << ".";
  includePaths << "../../build/";
  includePaths << "/usr/include/";
  includePaths << "/usr/include/qt4";
  includePaths << "/usr/lib64/gcc/x86_64-pc-linux-gnu/4.1.2/include/";
  includePaths << "/usr/lib/gcc/x86_64-pc-linux-gnu/4.1.2/include/g++-v4";
  includePaths << "/usr/lib64/gcc/x86_64-pc-linux-gnu/4.1.2/include/g++-v4/x86_64-pc-linux-gnu";

  qDebug() << "Start preprocessing: " << file.path();
  CppPreprocessor preproc; // = new CppPreprocessor(this);
  preproc.setProjectFiles(QStringList() << file.path());
  preproc.setIncludePaths(includePaths);
  //preproc->setFrameworkPaths(frameworkPaths());
  //preproc->setWorkingCopy(workingCopy);
  QString path = file.path();
  preproc.run(path);

  // Parse the file
  /*
  Control control;
  StringLiteral *fileId = control.findOrInsertFileName(file.path().toUtf8());
  TranslationUnit unit(&control, fileId);
  unit.setQtMocRunEnabled(true);

  QByteArray contents = preprocessor.readAll();
  unit.setSource(contents, contents.length());
  unit.parse();

  // Get semantic information out of it
  Semantic semantic(&control);
  TranslationUnitAST *ast = unit.ast()->asTranslationUnit();

  Scope globalScope;
  Semantic sem(&control);
  for (DeclarationAST *decl = ast->declarations; decl; decl = decl->next) 
  {
    sem.check(decl, &globalScope);
  }

  // Now lets see if we can find any issue.
  m_results = m_analyzer->analyze(globalScope);
  */
}

QList<Result> CheckEngine::results() const
{
  return m_results;
}
