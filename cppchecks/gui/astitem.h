#ifndef ASTTREEMODELITEM_H
#define ASTTREEMODELITEM_H

#include <AST.h>
#include <QtCore/QVariant>

using namespace CPlusPlus;

class Item
{
  public:
    Item(Item* parent = 0);
    virtual ~Item();

    void appendChild(Item *child);
    AST* ast() const;
    Item *child(int row);
    int childCount() const;
    int columnCount() const;
    virtual QVariant data(int column);
    virtual QVariant nameData();
    Item *parent() const;
    int row() const;
    void setAST(AST* ast);

  private:
    AST            *m_ast;
    QList<Item*>    m_childItems;
    QList<QVariant> m_itemData;
    Item           *m_parentItem;
};

#endif // ASTTREEMODELITEM_H
