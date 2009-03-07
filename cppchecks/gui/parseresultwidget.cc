#include "parseresultwidget.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QModelIndex>
#include <QtGui/QFileDialog>
#include <QtGui/QTextBlock>

#include <CppPreprocessor.h>
#include <Scope.h>
#include <TranslationUnit.h>

#include "asttreemodel.h"
#include "includestreemodel.h"
#include "messagetablemodel.h"
#include "asttreebuilder.h"
#include "ui_parseresultwidget.h"

ParseResultWidget::ParseResultWidget()
  : m_globals(0),
    m_includeTreeModel(0),
    m_ui(new Ui::ParseResultWidget()),
    m_selectedDoc(0)
{
  m_ui->setupUi(this);
  connect(m_ui->m_openFileButton, SIGNAL(clicked()), this, SLOT(openFile()));
  connect(m_ui->m_treeView, SIGNAL(clicked(QModelIndex const &)),
          this, SLOT(onIncludeClicked(QModelIndex const &)));
  connect(m_ui->m_preprocessedCheck, SIGNAL(stateChanged(int)),
          this, SLOT(onStateChanged(int)));
  connect(m_ui->m_treeCombo, SIGNAL(currentIndexChanged(int)),
          this, SLOT(onTreeTypeChanged(int)));

  // TODO: Read from config.
  QStringList includePaths;
  includePaths << QDir::current().path();
  includePaths << "/usr/include/";
  includePaths << "/usr/include/qt4";
  includePaths << "/usr/lib/gcc/i686-pc-linux-gnu/4.1.2/include";
  includePaths << "/usr/lib/gcc/i686-pc-linux-gnu/4.1.2/include/g++-v4";
  includePaths << "/usr/lib/gcc/i686-pc-linux-gnu/4.1.2/include/g++-v4/i686-pc-linux-gnu/";

  m_ui->m_includeDirsList->addItems(includePaths);
  m_ui->m_tabs->setEnabled(true);
}

ParseResultWidget::~ParseResultWidget()
{
  if (m_ui->m_treeView->model() != m_includeTreeModel)
    delete m_ui->m_treeView->model();

  delete m_globals;
  delete m_ui;
  delete m_includeTreeModel;
}

QAbstractItemModel *ParseResultWidget::buildASTModel() const
{
  m_selectedDoc->parse();

  ASTTreeModel* model = new ASTTreeModel();
  model->getRootItem()->setAST(m_selectedDoc->translationUnit()->ast());
  TreeBuilder builder(m_selectedDoc->translationUnit()->control());
  builder.setRootItem(model->getRootItem());

  m_selectedDoc->translationUnit()->ast()->asTranslationUnit()->accept(&builder);

  return model;
}

QAbstractItemModel *ParseResultWidget::buildSemanticModel()
{
  if (!m_globals)
  {
    qDebug() << "Building Scope";
    m_globals = new Scope(0);
    m_rootDoc->check(m_globals);
  }

  // TODO: Build a semantic tree model and return it.
  return 0;
}

QStringList ParseResultWidget::includePaths() const
{
  QStringList paths;

  for(int idx = 0; idx < m_ui->m_includeDirsList->count(); ++idx)
    paths << m_ui->m_includeDirsList->item(idx)->text();

  return paths;
}

void ParseResultWidget::onASTItemClicked(QModelIndex const &index)
{
  Item *item = static_cast<Item*>(index.internalPointer());
  TranslationUnit *unit = m_selectedDoc->translationUnit();

  unsigned line, column;
  StringLiteral *fileId = 0;
  unit->getTokenPosition(item->ast()->firstToken(), &line, &column, &fileId);
  qDebug() << "Line: " << line + 1 << " Column:" << column << "firstToken: " << item->ast()->firstToken() ;

  QTextCursor tc(m_ui->m_headerView->document()->findBlockByLineNumber(line));
  //tc.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor, column);
  tc.select(QTextCursor::LineUnderCursor);

  m_ui->m_headerView->setTextCursor(tc);
}

void ParseResultWidget::onIncludeClicked(QModelIndex const &index)
{
  m_selectedDoc = static_cast<CPlusPlus::Document*>(index.internalPointer());
  m_ui->m_messageTable->setModel(new MessageTableModel(m_selectedDoc->diagnosticMessages()));
  m_ui->m_messageTable->setColumnWidth(0, 40);

  onStateChanged(m_ui->m_preprocessedCheck->checkState());
}

void ParseResultWidget::onStateChanged(int state)
{
  if (state == Qt::Unchecked)
  {
    QFile header(m_selectedDoc->fileName());
    if (header.exists() && header.open(QIODevice::ReadOnly))
    {
      QString text = header.readAll();
      m_ui->m_headerView->setPlainText(text);
    }
    else
    {
      m_ui->m_headerView->setPlainText("<empty file>");
    }
  }
  else
  {
    m_ui->m_headerView->setPlainText(m_selectedDoc->source());
  }
}

void ParseResultWidget::onTreeTypeChanged(int index)
{
  switch(index)
  {
    case 0: // Include Tree
      m_ui->m_treeView->disconnect(this);
      connect(m_ui->m_treeView, SIGNAL(clicked(QModelIndex const &)),
              this, SLOT(onIncludeClicked(QModelIndex const &)));
      m_ui->m_treeView->setModel(m_includeTreeModel);
      break;
    case 1: // Abstract Syntax Tree
      if (!m_selectedDoc)
        return; // We can only create an AST when a document is selected.

      m_ui->m_treeView->disconnect(this);
      connect(m_ui->m_treeView, SIGNAL(clicked(QModelIndex const &)),
              this, SLOT(onASTItemClicked(QModelIndex const &)));

      if (m_ui->m_treeView->model() != m_includeTreeModel)
        delete m_ui->m_treeView->model();

      m_ui->m_treeView->setModel(buildASTModel());
      break;
    case 2: // Semantic Tree
      m_ui->m_treeView->disconnect(this);
      //connect(m_ui->m_treeView, SIGNAL(clicked(QModelIndex const &)),
      //        this, SLOT(onSemanticItemClicked(QModelIndex const &)));
      if (m_ui->m_treeView->model() != m_includeTreeModel)
        delete m_ui->m_treeView->model();

      buildSemanticModel();
      //m_ui->m_treeView->setModel(buildSemanticModel());
      break;
    default: // Invalid
      break;
  }
}

void ParseResultWidget::openFile()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 QDir::current().path(),
                                                 tr("C/C++ files (*.h *.hpp *.cc *.cpp)"));
  if (!fileName.isEmpty())
  {
    if (m_includeTreeModel)
    {
      delete m_includeTreeModel;
      m_includeTreeModel = 0;
    }

    CPlusPlus::CppPreprocessor preproc;
    preproc.setIncludePaths(includePaths());

    delete m_includeTreeModel;

    m_rootDoc = preproc.run(fileName);
    m_includeTreeModel = new IncludesTreeModel(m_rootDoc);
    m_ui->m_treeView->setModel(m_includeTreeModel);
  }
}
