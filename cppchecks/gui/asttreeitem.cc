#include "asttreeitem.h"

#include <QtCore/QDebug>

ASTTreeItem::ASTTreeItem(ASTTreeItem* parent)
{
  parentItem = parent;
}

ASTTreeItem::~ASTTreeItem()
{
}

void ASTTreeItem::appendChild(ASTTreeItem *child)
{
    childItems.append(child);
}

ASTTreeItem* ASTTreeItem::child(int row)
{
  return childItems.at(row);
}

int ASTTreeItem::childCount() const
{
    return childItems.count();
}

int ASTTreeItem::columnCount() const
{
    return 5;
}

QVariant ASTTreeItem::data(int column) const
{
if (1 == column)
  return ast()->firstToken();
if (2 == column)
  return ast()->lastToken();

return QVariant();
}

int ASTTreeItem::row() const
{
if (parentItem)
  return parentItem->childItems.indexOf(const_cast<ASTTreeItem*>(this));

return 0;
}

ASTTreeItem* ASTTreeItem::parent()
{
return parentItem;
}

AST* ASTTreeItem::ast() const
{
return _ast;
}

void ASTTreeItem::setAST(AST* ast)
{
_ast = ast;
}

QVariant getTranslationUnitASTData(TranslationUnitAST* ast, int column)
{
switch (column)
{
  case 0:
    return "TranslationUnitAST Item";
  case 1:
    return ast->firstToken();
  case 2:
    return ast->lastToken();
}

return "TU ast empty";
}

QVariant getSimpleDeclarationASTData(SimpleDeclarationAST* ast, int column)
{
switch (column)
{
  case 0:
    return "SimpleDeclarationAST Item";
  case 1:
    return ast->firstToken();
  case 2:
    return ast->lastToken();
}

return "TU ast empty";
}


QVariant getClassSpecifierASTData(ClassSpecifierAST* ast, int column)
{
switch (column)
{
  case 0:
    return "ClassSpecifierAST Item";
  case 1:
    return ast->firstToken();
  case 2:
    return ast->lastToken();
}

return "TU ast empty";
}

QVariant getSimpleNameASTData(SimpleNameAST* ast, int column)
{
switch (column)
{
  case 0:
    return "SimpleNameAST Item";
  case 1:
    return ast->firstToken();
  case 2:
    return ast->lastToken();
  case 3:
    return "name";
}

return "SimpleNameAST empty";
}

QVariant getSimpleSpecifierASTData(SimpleSpecifierAST* ast, int column)
{
  switch (column)
    {
        case 0:
          return "SimpleSpecifierAST Item";
        case 1:
          return ast->firstToken();
        case 2:
          return ast->lastToken();


    }

    return "SimpleSpecifierAST empty";
}
