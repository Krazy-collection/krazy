#include "asttreemodel.h"

#ifdef Q_CC_GNU
#include <cxxabi.h>
#endif

#include <parser/AST.h>
#include <parser/TranslationUnit.h>

#include <QtCore/QDebug>
#include <QtGui/QStandardItem>

using namespace CPlusPlus;

ASTTreeModel::ASTTreeModel(TranslationUnit *translationUnit)
  : ASTVisitor(translationUnit->control())
{
  setHorizontalHeaderItem(0, new QStandardItem("AST Node Type"));
  accept(translationUnit->ast());
}

ASTTreeModel::~ASTTreeModel()
{}

/* virtual */ void ASTTreeModel::postVisit(CPlusPlus::AST* ast)
{
  if (!m_items.empty())
    m_items.pop();
}

/* virtual */ bool ASTTreeModel::preVisit(CPlusPlus::AST *ast)
{
#ifdef Q_CC_GNU
  QStandardItem *item = new QStandardItem(abi::__cxa_demangle(
    typeid(*ast).name(), 0, 0, 0) + 11);
#else
  QStandardItem *item = new QStandardItem((ast).name());
#endif

  //item->setData(var, DocumentRole); // NOTE: additional information can be added here.

  if (m_items.empty())
    invisibleRootItem()->appendRow(item);
  else
    m_items.top()->appendRow(item);

  m_items.push(item);

  return true;
}
