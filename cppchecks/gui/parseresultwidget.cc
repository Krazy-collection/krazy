#include "parseresultwidget.h"

#include <QtCore/QDir>
#include <QtGui/QFileDialog>

#include "CppPreprocessor.h"
#include "includestreemodel.h"
#include "ui_parseresultwidget.h"

ParseResultWidget::ParseResultWidget()
  : m_ui(new Ui::ParseResultWidget())
{
  m_ui->setupUi(this);
  connect(m_ui->m_openFileButton, SIGNAL(clicked()), this, SLOT(openFile()));
}

ParseResultWidget::~ParseResultWidget()
{
  delete m_ui;
}

void ParseResultWidget::openFile()
{
  QStringList includePaths;
  includePaths << ".";
  includePaths << "../../build/";
  includePaths << "/usr/include/";
  includePaths << "/usr/include/qt4";
  includePaths << "/usr/lib/gcc/x86_64-pc-linux-gnu/4.1.2/include";
  includePaths << "/usr/lib/gcc/x86_64-pc-linux-gnu/4.1.2/include/g++-v4";
  includePaths << "/usr/lib64/gcc/x86_64-pc-linux-gnu/4.1.2/include/g++-v4/x86_64-pc-linux-gnu";

  CPlusPlus::CppPreprocessor preproc;
  preproc.setIncludePaths(includePaths);

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 QDir::current().path(),
                                                 tr("C/C++ files (*.h *.hpp *.cc *.cpp)"));

  m_ui->m_treeView->setModel(new IncludesTreeModel(preproc.run(fileName)));
}
