#include "astitem.h"

Item::Item(Item* parent)
{
  parentItem = parent;
}

Item::~Item()
{
  qDeleteAll(childItems);
}

void Item::appendChild(Item *child)
{
  childItems.append(child);
}

Item* Item::child(int row)
{
  return childItems.at(row);
}


int Item::childCount() const
{
  return childItems.count();
}

int Item::columnCount() const
{
  return 1;
}

//warning virtual method!
QVariant Item::nameData()
{
  return "Item";
}

QVariant Item::data(int column)
{
  if (1 == column)
    return ast()->firstToken();
  if (2 == column)
    return ast()->lastToken();

  return QVariant();
}

int Item::row() const
{
  if (parentItem)
    return parentItem->childItems.indexOf(const_cast<Item*>(this));

  return 0;
}

Item* Item::parent()
{
  return parentItem;
}

AST* Item::ast() const
{
  return _ast;
}

void Item::setAST(AST* ast)
{
  _ast = ast;
}
