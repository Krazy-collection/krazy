#ifndef TREEBUILDER_H
#define TREEBUILDER_H

#include <parser/ASTVisitor.h>

#include "astitem.h"

using namespace CPlusPlus;

class TreeBuilder : public ASTVisitor
{
  public:
    virtual bool preVisit(AST *ast);
    virtual void postVisit(AST* ast);
    TreeBuilder(Control *control);
    ~TreeBuilder();
    void addDummyNode(const char * str);
    void setRootItem(Item* item);
    void appendItem(Item* item, AST* ast);

  private:
    Item* m_rootItem;
    Item* m_currentItem;
};

#endif // TREEBUILDER_H
