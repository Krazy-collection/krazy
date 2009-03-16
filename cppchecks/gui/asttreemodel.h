#ifndef ASTTREEMODEL_H
#define ASTTREEMODEL_H

#include <parser/ASTVisitor.h>
#include <parser/CPlusPlusForwardDeclarations.h>

#include <QtCore/QStack>
#include <QtGui/QStandardItemModel>

class ASTTreeModel : public QStandardItemModel, private CPlusPlus::ASTVisitor
{
  public: // Functions
    /**
     * Creates a TreeModel for given TranslationUnit. This class will not take
     * ownership of @param translationUnit and you should make shure that this
     * model is not used after deletion of @param translationUnit.
     */
    ASTTreeModel(CPlusPlus::TranslationUnit *translationUnit);

    ~ASTTreeModel();

  private: // Functions
    virtual void postVisit(CPlusPlus::AST* ast);

    virtual bool preVisit(CPlusPlus::AST *ast);

  private: // Members
    QStack<QStandardItem*> m_items; // For building the tree model.
};

#endif // ASTTREEMODEL_H
