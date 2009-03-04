#include "astitem.h"

Item::Item(Item* parent) : m_ast(0), m_parentItem(parent)
{
}

Item::~Item()
{
  qDeleteAll(m_childItems);
}

AST* Item::ast() const
{
  return m_ast;
}

void Item::appendChild(Item *child)
{
  m_childItems.append(child);
}

Item* Item::child(int row)
{
  return m_childItems.at(row);
}

int Item::childCount() const
{
  return m_childItems.count();
}

int Item::columnCount() const
{
  return 1;
}

QVariant Item::data(int column)
{
  if (1 == column)
    return ast()->firstToken();
  if (2 == column)
    return ast()->lastToken();

  return QVariant();
}

/* virtual */ QVariant Item::nameData()
{
  return "Item";
}

Item* Item::parent() const
{
  return m_parentItem;
}

int Item::row() const
{
  if (m_parentItem)
    return m_parentItem->m_childItems.indexOf(const_cast<Item*>(this));

  return 0;
}

void Item::setAST(AST* ast)
{
  m_ast = ast;
}
