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

int main(int argc, char **argv)
{
  if (argc != 2)
    return 1; // Require a file argument.

  QUrl file(argv[1]);

  QStringList includePaths;
  includePaths << "."; // Current work directory
  includePaths << "/usr/include/";
  // Change this to our system include paths (e.g. look at g++ -version)
  includePaths << "/usr/include/qt4";
  includePaths << "/usr/lib/gcc/x86_64-pc-linux-gnu/4.1.2/include";
  includePaths << "/usr/lib/gcc/x86_64-pc-linux-gnu/4.1.2/include/g++-v4";
  includePaths << "/usr/lib64/gcc/x86_64-pc-linux-gnu/4.1.2/include/g++-v4/x86_64-pc-linux-gnu";

  CppPreprocessor preproc;
  preproc.setIncludePaths(includePaths);
  QString path = file.path();
  QList<CPlusPlus::Document::Ptr> documents = preproc.run(path);

  // TODO: We now build one big tree for all the files. This is not necessary
  //       for more simple checks so we might want to make that optional.
  // Parse the files and build the semantic tree
  Control control;
  Scope globalScope;
  Semantic sem(&control);

  foreach(CPlusPlus::Document::Ptr const &doc, documents)
  {
    StringLiteral *fileId = control.findOrInsertFileName(file.path().toUtf8());
    TranslationUnit unit(&control, fileId);
    unit.setQtMocRunEnabled(true);
    unit.setSource(doc->source(), doc->source().length());
    unit.parse();

    // Get semantic information out of it
    TranslationUnitAST *ast = unit.ast()->asTranslationUnit();
    for (DeclarationAST *decl = ast->declarations; decl; decl = decl->next) 
    {
      sem.check(decl, &globalScope);
    }
  }

  // At this point globalScope should contain all the symbols. and you can do
  // whatever you want with it.
}

