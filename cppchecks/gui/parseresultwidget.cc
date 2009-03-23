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
#include "parseresultwidget.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QModelIndex>
#include <QtGui/QFileDialog>
#include <QtGui/QTextBlock>

#include <cppmodel/cpppreprocessor.h>
#include <parser/Scope.h>
#include <parser/TranslationUnit.h>

#include "asttreemodel.h"
#include "dumpast.h"
#include "dumpscope.h"
#include "includestreemodel.h"
#include "messagetablemodel.h"
#include "symboltreemodel.h"
#include "ui_parseresultwidget.h"

using namespace CPlusPlus;
using namespace CppModel;

/// ParseResultWidget :: public functions

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
  connect(m_ui->m_exportAST, SIGNAL(clicked()),
          this, SLOT(exportAST()));
  connect(m_ui->m_exportScope, SIGNAL(clicked()),
          this, SLOT(exportScope()));

  // TODO: Read from config.
  QStringList includePaths;
  includePaths << QDir::current().path();
  includePaths << "/usr/include/";
  includePaths << "/usr/include/qt4";

  includePaths << "/usr/lib64/gcc/x86_64-pc-linux-gnu/4.1.2/include";
  includePaths << "/usr/lib64/gcc/x86_64-pc-linux-gnu/4.1.2/include/g++-v4";
  includePaths << "/usr/lib64/gcc/x86_64-pc-linux-gnu/4.1.2/include/g++-v4/x86_64-pc-linux-gnu";
  includePaths << "/usr/lib/gcc/i686-pc-linux-gnu/4.1.2/include";
  includePaths << "/usr/lib/gcc/i686-pc-linux-gnu/4.1.2/include/g++-v4";
  includePaths << "/usr/lib/gcc/i686-pc-linux-gnu/4.1.2/include/g++-v4/i686-pc-linux-gnu";

  m_ui->m_globalIncludeDirsList->addItems(includePaths);
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

/// ParseResultWidget :: protected slots

void ParseResultWidget::exportAST()
{
  if (!m_selectedDoc)
    return;

  QString fileName = QFileDialog::getSaveFileName(this, tr("Export AST")
                            , m_selectedDoc->fileName() + ".ast"
                            , tr("AST Files (*.ast)"));
  QFile result(fileName);
  result.remove();
  result.open(QIODevice::WriteOnly);

  DumpAST dumper(&result, m_selectedDoc->translationUnit()->control());
  dumper(m_selectedDoc->translationUnit()->ast());

  result.close();
}

void ParseResultWidget::exportScope()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export Scope")
                            , m_rootDoc->fileName() + ".scope"
                            , tr("Scope Files (*.scope)"));
  QFile result(fileName);
  result.remove();
  result.open(QIODevice::WriteOnly);
  DumpScope dumper(&result);
  dumper(*m_globals);
}

void ParseResultWidget::onASTItemClicked(QModelIndex const &index)
{
  /*
  Item *item = static_cast<Item*>(index.internalPointer());
  TranslationUnit *unit = m_selectedDoc->translationUnit();

  unsigned line, column;
  StringLiteral *fileId = 0;
  unit->getTokenPosition(item->ast()->firstToken(), &line, &column, &fileId);
  //qDebug() << "Line: " << line + 1 << " Column:" << column << "firstToken: " 
  //        << item->ast()->firstToken() ;

  QTextCursor tc(m_ui->m_headerView->document()->findBlockByLineNumber(line));
  //tc.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor, column);
  tc.select(QTextCursor::LineUnderCursor);

  m_ui->m_headerView->setTextCursor(tc);
  */
}

void ParseResultWidget::onIncludeClicked(QModelIndex const &index)
{
  m_selectedDoc = index.data(IncludesTreeModel::DocumentRole).value<Document::Ptr>();
  m_ui->m_messageTable->setModel(new MessageTableModel(m_selectedDoc->diagnosticMessages()));
  m_ui->m_messageTable->setColumnWidth(0, 40);
  onStateChanged(m_ui->m_preprocessedCheck->checkState());
}

void ParseResultWidget::onStateChanged(int state)
{
  QByteArray source;

  QFile header(m_selectedDoc->absoluteFileName());
  if (header.exists() && header.open(QIODevice::ReadOnly))
    source = header.readAll();
  else
  {
    m_ui->m_headerView->setPlainText("<empty file>");
    return;
  }

  if (state == Qt::Unchecked)
    m_ui->m_headerView->setPlainText(source);
  else
  {
    // TODO: Fixme
    CppPreprocessor preproc;
    QByteArray preprocessedSource = preproc(m_selectedDoc->absoluteFileName());
    m_ui->m_headerView->setPlainText(preprocessedSource);
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

      if (m_selectedDoc)
        m_ui->m_treeView->setModel(new ASTTreeModel(m_selectedDoc->translationUnit()));
      break;
    case 2: // Scope Tree
      m_ui->m_treeView->disconnect(this);
      //connect(m_ui->m_treeView, SIGNAL(clicked(QModelIndex const &)),
      //        this, SLOT(onSemanticItemClicked(QModelIndex const &)));
      if (m_ui->m_treeView->model() != m_includeTreeModel)
        delete m_ui->m_treeView->model();

      m_ui->m_treeView->setModel(new SymbolTreeModel(m_rootDoc->globalNamespace()));
      m_ui->m_treeView->setColumnWidth(0, 175);
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

    // TODO: Also make it possible to add local include paths and to modify those
    //       paths in the gui.
    CppPreprocessor preproc;
    preproc.setLocalIncludePaths(QStringList() << QFileInfo(fileName).absolutePath());
    preproc.setGlobalIncludePaths(includePaths());

    delete m_includeTreeModel;

    m_rootDoc = preproc(fileName);
    m_includeTreeModel = new IncludesTreeModel(m_rootDoc);
    m_ui->m_treeView->setModel(m_includeTreeModel);
  }
}

/// ParseResultWidget :: private functions

QStringList ParseResultWidget::includePaths() const
{
  QStringList paths;
  // TODO: Make difference between global/local includes.
  for(int idx = 0; idx < m_ui->m_globalIncludeDirsList->count(); ++idx)
    paths << m_ui->m_globalIncludeDirsList->item(idx)->text();

  return paths;
}
