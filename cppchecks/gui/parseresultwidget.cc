#include "parseresultwidget.h"

#include <QtCore/QDir>
#include <QtCore/QModelIndex>
#include <QtGui/QFileDialog>

#include "CppPreprocessor.h"
#include "includestreemodel.h"
#include "messagetablemodel.h"
#include "ui_parseresultwidget.h"

ParseResultWidget::ParseResultWidget()
  : m_ui(new Ui::ParseResultWidget())
{
  m_ui->setupUi(this);
  connect(m_ui->m_openFileButton, SIGNAL(clicked()), this, SLOT(openFile()));
  connect(m_ui->m_treeView, SIGNAL(clicked(QModelIndex const &)),
          this, SLOT(onClicked(QModelIndex const &)));

  // TODO: Read from config.
  QStringList includePaths;
  includePaths << QDir::current().path();
  includePaths << "/usr/include/";
  includePaths << "/usr/include/qt4";
  includePaths << "/usr/lib/gcc/x86_64-pc-linux-gnu/4.1.2/include";
  includePaths << "/usr/lib/gcc/x86_64-pc-linux-gnu/4.1.2/include/g++-v4";
  includePaths << "/usr/lib64/gcc/x86_64-pc-linux-gnu/4.1.2/include/g++-v4/x86_64-pc-linux-gnu";

  m_ui->m_includeDirsList->addItems(includePaths);
  m_ui->m_tabs->setEnabled(true);
}

ParseResultWidget::~ParseResultWidget()
{
  delete m_ui;
}

QStringList ParseResultWidget::includePaths() const
{
  QStringList paths;

  for(int idx = 0; idx < m_ui->m_includeDirsList->count(); ++idx)
    paths << m_ui->m_includeDirsList->item(idx)->text();

  return paths;
}

void ParseResultWidget::onClicked(QModelIndex const &index)
{
  CPlusPlus::Document *doc = static_cast<CPlusPlus::Document*>(index.internalPointer());
  m_ui->m_messageTable->setModel(new MessageTableModel(doc->diagnosticMessages()));
  m_ui->m_messageTable->setColumnWidth(0, 40);
}

void ParseResultWidget::openFile()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 QDir::current().path(),
                                                 tr("C/C++ files (*.h *.hpp *.cc *.cpp)"));
  CPlusPlus::CppPreprocessor preproc;
  preproc.setIncludePaths(includePaths());
  m_ui->m_treeView->setModel(new IncludesTreeModel(preproc.run(fileName)));
}
