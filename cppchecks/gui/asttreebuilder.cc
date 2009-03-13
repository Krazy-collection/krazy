#include "asttreebuilder.h"

#include <parser/AST.h>

#include <QtCore/QDebug>
#include <QtCore/QString>

#include "astitem.h"

//using namespace CPlusPlus;

TreeBuilder::TreeBuilder(Control *control):ASTVisitor(control)
{}

TreeBuilder::~TreeBuilder()
{}

bool TreeBuilder::preVisit(AST* ast)
{
  Item* item = new Item(m_currentItem);
  item->setAST(ast);
  m_currentItem->appendChild(item);
  m_currentItem = item;
  return true;
}

void TreeBuilder::postVisit(AST* ast)
{
  if (m_currentItem->parent())
    m_currentItem = m_currentItem->parent();
}

void TreeBuilder::addDummyNode(const char * str)
{
  qDebug() << str;
}

void TreeBuilder::setRootItem(Item* item)
{
  m_rootItem = item;
  m_currentItem = m_rootItem;
}

