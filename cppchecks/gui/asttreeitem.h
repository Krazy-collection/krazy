#ifndef ASTASTTreeItem_H
#define ASTASTTreeItem_H

#include <QtCore/QList>
#include <QtCore/QVariant>

#include <AST.h>

using namespace CPlusPlus;

class ASTTreeItem
{
 public:
     //ASTASTTreeItem(const QList<QVariant> &data, ASTTreeItem *parent = 0);
     ASTTreeItem(ASTTreeItem *parent = 0);
     ~ASTTreeItem();

     enum ItemType
     {
        TranslationUnitItem,
        SimpleDeclarationItem,
        ClassSpecifierItem,
        SimpleNameItem,
        SimpleSpecifierItem

     };

     void appendChild(ASTTreeItem *child);

     ASTTreeItem *child(int row);
     int childCount() const;
     int columnCount() const;
     QVariant data(int column) const;
     int row() const;
     ASTTreeItem *parent();
     AST* ast() const;
     void setAST(AST* ast);


      private:
     QList<ASTTreeItem*> childItems;
     QList<QVariant> itemData;
     ASTTreeItem *parentItem;
     AST* _ast;


     //QVariant getASTData(MyAST1* ast, int column);
     //QVariant getASTData(MyAST2* ast, int column);


};

//QVariant getASTData(AST* ast, int column);
QVariant getTranslationUnitASTData(TranslationUnitAST* ast, int column);
QVariant getSimpleDeclarationASTData(SimpleDeclarationAST* ast, int column);
QVariant getClassSpecifierASTData(ClassSpecifierAST* ast, int column);
QVariant getSimpleNameASTData(SimpleNameAST* ast, int column);
QVariant getSimpleSpecifierASTData(SimpleSpecifierAST* ast, int column);

#endif // ASTASTTreeItem_H
