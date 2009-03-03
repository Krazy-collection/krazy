#ifndef ASTTREEMODELITEM_H
#define ASTTREEMODELITEM_H

#include <AST.h>
#include <QtCore/QVariant>

using namespace CPlusPlus;

class CPLUSPLUS_EXPORT Item
{
  public:
    Item(Item* parent = 0);
    virtual ~Item();

    void appendChild(Item *child);

    Item *child(int row);
    int childCount() const;
    int columnCount() const;

    int row() const;
    Item *parent();
    AST* ast() const;
    void setAST(AST* ast);
    virtual QVariant nameData();
    virtual QVariant data(int column);

  private:
    QList<Item*> childItems;
    QList<QVariant> itemData;
    Item *parentItem;
    AST* _ast;
};

class CPLUSPLUS_EXPORT SpecifierItem : public Item
{
  public:
    SpecifierItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "SpecifierItem";
    }
};

class CPLUSPLUS_EXPORT SimpleSpecifierItem : public SpecifierItem
{
  public:
    SimpleSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "SimpleSpecifierItem";
    }
};

class CPLUSPLUS_EXPORT AttributeSpecifierItem: public SpecifierItem
{
  public:
    AttributeSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "AttributeSpecifierItem";
    }
};

class CPLUSPLUS_EXPORT AttributeItem: public Item
{
  public:
    AttributeItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "AttributeItem";
    }
};

class CPLUSPLUS_EXPORT TypeofSpecifierItem: public SpecifierItem
{
  public:
    TypeofSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "TypeofSpecifierItem";
    }
};

class CPLUSPLUS_EXPORT StatementItem: public Item
{
  public:
    StatementItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "StatementItem";
    }
};

class CPLUSPLUS_EXPORT ExpressionItem : public Item
{
  public:
    ExpressionItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "ExpressionItem";
    }
};

class CPLUSPLUS_EXPORT DeclarationItem : public Item
{
  public:
    DeclarationItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "DeclarationItem";
    }
};

class CPLUSPLUS_EXPORT CoreDeclaratorItem: public Item
{
  public:
    CoreDeclaratorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "CoreDeclaratorItem";
    }
};

class CPLUSPLUS_EXPORT PostfixDeclaratorItem: public Item
{
  public:
    PostfixDeclaratorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "PostfixDeclaratorItem";
    }
};

class CPLUSPLUS_EXPORT DeclaratorItem : public Item
{
  public:
    DeclaratorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "DeclaratorItem";
    }
};

class CPLUSPLUS_EXPORT ExpressionListItem : public ExpressionItem
{
  public:
    ExpressionListItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "ExpressionListItem";
    }
};

class CPLUSPLUS_EXPORT SimpleDeclarationItem : public DeclarationItem
{
  public:
    SimpleDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "SimpleDeclarationItem";
    }
};

class CPLUSPLUS_EXPORT EmptyDeclarationItem: public DeclarationItem
{
  public:
    EmptyDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "EmptyDeclarationItem";
    }
};

class CPLUSPLUS_EXPORT AccessDeclarationItem: public DeclarationItem
{
  public:
    AccessDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "AccessDeclarationItem";
    }
};

class CPLUSPLUS_EXPORT AsmDefinitionItem: public DeclarationItem
{
  public:
    AsmDefinitionItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "AsmDefinitionItem";
    }
};

class CPLUSPLUS_EXPORT BaseSpecifierItem: public Item
{
  public:
    BaseSpecifierItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "BaseSpecifierItem";
    }
};

class CPLUSPLUS_EXPORT CompoundLiteralItem: public ExpressionItem
{
  public:
    CompoundLiteralItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "CompoundLiteralItem";
    }
};

class CPLUSPLUS_EXPORT QtMethodItem: public ExpressionItem
{
  public:
    QtMethodItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "QtMethodItem";
    }
};

class CPLUSPLUS_EXPORT BinaryExpressionItem: public ExpressionItem
{
  public:
    BinaryExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "BinaryExpressionItem";
    }
};

class CPLUSPLUS_EXPORT CastExpressionItem: public ExpressionItem
{
  public:
    CastExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "CastExpressionItem";
    }
};

class CPLUSPLUS_EXPORT ClassSpecifierItem: public SpecifierItem
{
  public:
    ClassSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "ClassSpecifierItem";
    }
};

class CPLUSPLUS_EXPORT CaseStatementItem : public StatementItem
{
  public:
    CaseStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "CaseStatementItem";
    }
};

class CPLUSPLUS_EXPORT CompoundStatementItem : public StatementItem
{
  public:
    CompoundStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "CompoundStatementItem";
    }
};

class CPLUSPLUS_EXPORT ConditionItem: public ExpressionItem
{
  public:
    ConditionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "ConditionItem";
    }
};

class CPLUSPLUS_EXPORT ConditionalExpressionItem: public ExpressionItem
{
  public:
    ConditionalExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "ConditionalExpressionItem";
    }
};

class CPLUSPLUS_EXPORT CppCastExpressionItem: public ExpressionItem
{
  public:
    CppCastExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "CppCastExpressionItem";
    }
};

class CPLUSPLUS_EXPORT CtorInitializerItem: public Item
{
  public:
    CtorInitializerItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "CtorInitializerItem";
    }
};

class CPLUSPLUS_EXPORT DeclarationStatementItem: public StatementItem
{
  public:
    DeclarationStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "DeclarationStatementItem";
    }
};

class CPLUSPLUS_EXPORT DeclaratorIdItem: public CoreDeclaratorItem
{
  public:
    DeclaratorIdItem(Item *parent = 0) : CoreDeclaratorItem(parent) {}

    virtual QVariant nameData()
    {
      return "DeclaratorIdItem";
    }
};

class CPLUSPLUS_EXPORT NestedDeclaratorItem: public CoreDeclaratorItem
{
  public:
    NestedDeclaratorItem(Item *parent = 0) : CoreDeclaratorItem(parent) {}

    virtual QVariant nameData()
    {
      return "NestedDeclaratorItem";
    }
};

class CPLUSPLUS_EXPORT FunctionDeclaratorItem: public PostfixDeclaratorItem
{
  public:
    FunctionDeclaratorItem(Item *parent = 0) : PostfixDeclaratorItem(parent) {}

    virtual QVariant nameData()
    {
      return "FunctionDeclaratorItem";
    }
};

class CPLUSPLUS_EXPORT ArrayDeclaratorItem: public PostfixDeclaratorItem
{
  public:
    ArrayDeclaratorItem(Item *parent = 0) : PostfixDeclaratorItem(parent) {}

    virtual QVariant nameData()
    {
      return "ArrayDeclaratorItem";
    }
};

class CPLUSPLUS_EXPORT DeclaratorListItem: public Item
{
  public:
    virtual QVariant nameData()
    {
      return "DeclaratorListItem";
    }
};

class CPLUSPLUS_EXPORT DeleteExpressionItem: public ExpressionItem
{
  public:
    virtual QVariant nameData()
    {
      return "DeleteExpressionItem";
    }
};

class CPLUSPLUS_EXPORT DoStatementItem: public StatementItem
{
  public:
    DoStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "DoStatementItem";
    }
};

class CPLUSPLUS_EXPORT NamedTypeSpecifierItem: public SpecifierItem
{
  public:
    NamedTypeSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "NamedTypeSpecifierItem";
    }
};

class CPLUSPLUS_EXPORT ElaboratedTypeSpecifierItem : public SpecifierItem
{
  public:
    ElaboratedTypeSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "ElaboratedTypeSpecifierItem";
    }
};

class CPLUSPLUS_EXPORT EnumSpecifierItem: public SpecifierItem
{
  public:
    EnumSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "EnumSpecifierItem";
    }
};

class CPLUSPLUS_EXPORT EnumeratorItem: public Item
{
  public:
    EnumeratorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "EnumeratorItem";
    }
};

class CPLUSPLUS_EXPORT ExceptionDeclarationItem: public DeclarationItem
{
  public:
    ExceptionDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "ExceptionDeclarationItem";
    }
};

class CPLUSPLUS_EXPORT ExceptionSpecificationItem: public Item
{
  public:
    ExceptionSpecificationItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "ExceptionSpecificationItem";
    }
};

class CPLUSPLUS_EXPORT ExpressionOrDeclarationStatementItem: public StatementItem
{
  public:
    ExpressionOrDeclarationStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "ExpressionOrDeclarationStatementItem";
    }
};

class CPLUSPLUS_EXPORT ExpressionStatementItem: public StatementItem
{
  public:
    ExpressionStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "ExpressionStatementItem";
    }
};

class CPLUSPLUS_EXPORT FunctionDefinitionItem: public DeclarationItem
{
  public:
    FunctionDefinitionItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "FunctionDefinitionItem";
    }
};

class CPLUSPLUS_EXPORT ForStatementItem: public StatementItem
{
  public:
    ForStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "ForStatementItem";
    }
};

class CPLUSPLUS_EXPORT IfStatementItem: public StatementItem
{
  public:
    IfStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "IfStatementItem";
    }
};

class CPLUSPLUS_EXPORT ArrayInitializerItem: public ExpressionItem
{
  public:
    ArrayInitializerItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "ArrayInitializerItem";
    }
};

class CPLUSPLUS_EXPORT LabeledStatementItem: public StatementItem
{
  public:
    LabeledStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "LabeledStatementItem";
    }
};

class CPLUSPLUS_EXPORT LinkageBodyItem: public DeclarationItem
{
  public:
    LinkageBodyItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "LinkageBodyItem";
    }
};

class CPLUSPLUS_EXPORT LinkageSpecificationItem: public DeclarationItem
{
  public:
    LinkageSpecificationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "LinkageSpecificationItem";
    }
};

class CPLUSPLUS_EXPORT MemInitializerItem: public Item
{
  public:
    MemInitializerItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "MemInitializerItem";
    }
};

class CPLUSPLUS_EXPORT NameItem: public ExpressionItem
{
  public:
    NameItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "NameItem";
    }
};

class CPLUSPLUS_EXPORT NestedNameSpecifierItem: public Item
{
  public:
    NestedNameSpecifierItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "NestedNameSpecifierItem";
    }
};

class CPLUSPLUS_EXPORT QualifiedNameItem: public NameItem
{
  public:
    QualifiedNameItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "QualifiedNameItem";
    }
};

class CPLUSPLUS_EXPORT OperatorFunctionIdItem: public NameItem
{
  public:
    OperatorFunctionIdItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "OperatorFunctionIdItem";
    }
};

class CPLUSPLUS_EXPORT ConversionFunctionIdItem: public NameItem
{
  public:
    ConversionFunctionIdItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "ConversionFunctionIdItem";
    }
};

class CPLUSPLUS_EXPORT SimpleNameItem: public NameItem
{
  public:
    SimpleNameItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "SimpleNameItem";
    }
};

class CPLUSPLUS_EXPORT DestructorNameItem: public NameItem
{
  public:
    DestructorNameItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "DestructorNameItem";
    }
};

class CPLUSPLUS_EXPORT TemplateIdItem: public NameItem
{
  public:
    TemplateIdItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "TemplateIdItem";
    }
};

class CPLUSPLUS_EXPORT NamespaceItem: public DeclarationItem
{
  public:
    NamespaceItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "NamespaceItem";
    }
};

class CPLUSPLUS_EXPORT NamespaceAliasDefinitionItem: public DeclarationItem
{
  public:
    NamespaceAliasDefinitionItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "NamespaceAliasDefinitionItem";
    }
};

class CPLUSPLUS_EXPORT NewPlacementItem: public Item
{
  public:
    NewPlacementItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "NewPlacementItem";
    }
};

class CPLUSPLUS_EXPORT NewArrayDeclaratorItem: public Item
{
  public:
    NewArrayDeclaratorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "NewArrayDeclaratorItem";
    }
};

class CPLUSPLUS_EXPORT NewExpressionItem: public ExpressionItem
{
  public:
    NewExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "NewExpressionItem";
    }
};

class CPLUSPLUS_EXPORT NewInitializerItem: public Item
{
  public:
    NewInitializerItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "NewInitializerItem";
    }
};

class CPLUSPLUS_EXPORT NewTypeIdItem: public Item
{
  public:
    NewTypeIdItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "NewTypeIdItem";
    }
};

class CPLUSPLUS_EXPORT OperatorItem: public Item
{
  public:
    OperatorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "OperatorItem";
    }
};

class CPLUSPLUS_EXPORT ParameterDeclarationItem: public DeclarationItem
{
  public:
    ParameterDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
        return "ParameterDeclarationItem";
    }
};

class CPLUSPLUS_EXPORT ParameterDeclarationClauseItem: public Item
{
  public:
    ParameterDeclarationClauseItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "ParameterDeclarationClauseItem";
    }
};

class CPLUSPLUS_EXPORT PostfixItem: public Item
{
  public:
    PostfixItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "PostfixItem";
    }
};

class CPLUSPLUS_EXPORT CallItem : public PostfixItem
{
  public:
    CallItem(Item *parent = 0) : PostfixItem(parent) {}

    virtual QVariant nameData()
    {
        return "CallItem";
    }
};

class CPLUSPLUS_EXPORT ArrayAccessItem: public PostfixItem
{
  public:
    ArrayAccessItem(Item *parent = 0) : PostfixItem(parent) {}

    virtual QVariant nameData()
    {
        return "ArrayAccessItem";
    }
};

class CPLUSPLUS_EXPORT PostIncrDecrItem: public PostfixItem
{
  public:
    PostIncrDecrItem(Item *parent = 0) : PostfixItem(parent) {}

    virtual QVariant nameData()
    {
        return "PostIncrDecrItem";
    }
};

class CPLUSPLUS_EXPORT MemberAccessItem: public PostfixItem
{
  public:
    MemberAccessItem(Item *parent = 0) : PostfixItem(parent) {}

    virtual QVariant nameData()
    {
        return "MemberAccessItem";
    }
};

class CPLUSPLUS_EXPORT TypeidExpressionItem: public ExpressionItem
{
  public:
    TypeidExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "TypeidExpressionItem";
    }
};

class CPLUSPLUS_EXPORT TypenameCallExpressionItem: public ExpressionItem
{
  public:
    TypenameCallExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "TypenameCallExpressionItem";
    }
};

class CPLUSPLUS_EXPORT TypeConstructorCallItem: public ExpressionItem
{
  public:
    TypeConstructorCallItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "TypeConstructorCallItem";
    }
};

class CPLUSPLUS_EXPORT PostfixExpressionItem: public ExpressionItem
{
  public:
    PostfixExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "PostfixExpressionItem";
    }
};

class CPLUSPLUS_EXPORT PtrOperatorItem: public Item
{
  public:
    PtrOperatorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "PtrOperatorItem";
    }
};

class CPLUSPLUS_EXPORT PointerToMemberItem: public PtrOperatorItem
{
  public:
    PointerToMemberItem(Item *parent = 0) : PtrOperatorItem(parent) {}

    virtual QVariant nameData()
    {
        return "PointerToMemberItem";
    }
};

class CPLUSPLUS_EXPORT PointerItem: public PtrOperatorItem
{
  public:
    PointerItem(Item *parent = 0) : PtrOperatorItem(parent) {}

    virtual QVariant nameData()
    {
        return "PointerItem";
    }
};

class CPLUSPLUS_EXPORT ReferenceItem: public PtrOperatorItem
{
  public:
    ReferenceItem(Item *parent = 0) : PtrOperatorItem(parent) {}

    virtual QVariant nameData()
    {
        return "ReferenceItem";
    }
};

class CPLUSPLUS_EXPORT BreakStatementItem: public StatementItem
{
  public:
    BreakStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
        return "BreakStatementItem";
    }
};

class CPLUSPLUS_EXPORT ContinueStatementItem: public StatementItem
{
  public:
    ContinueStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
        return "ContinueStatementItem";
    }
};

class CPLUSPLUS_EXPORT GotoStatementItem: public StatementItem
{
  public:
    GotoStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
        return "GotoStatementItem";
    }
};

class CPLUSPLUS_EXPORT ReturnStatementItem: public StatementItem
{
  public:
    ReturnStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
        return "ReturnStatementItem";
    }
};

class CPLUSPLUS_EXPORT SizeofExpressionItem: public ExpressionItem
{
  public:
    SizeofExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "SizeofExpressionItem";
    }
};

class CPLUSPLUS_EXPORT NumericLiteralItem: public ExpressionItem
{
  public:
    NumericLiteralItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "NumericLiteralItem";
    }
};

class CPLUSPLUS_EXPORT BoolLiteralItem: public ExpressionItem
{
  public:
    BoolLiteralItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "BoolLiteralItem";
    }
};

class CPLUSPLUS_EXPORT ThisExpressionItem: public ExpressionItem
{
  public:
    ThisExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "ThisExpressionItem";
    }
};

class CPLUSPLUS_EXPORT NestedExpressionItem: public ExpressionItem
{
  public:
    NestedExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "NestedExpressionItem";
    }
};

class CPLUSPLUS_EXPORT StringLiteralItem: public ExpressionItem
{
  public:
    StringLiteralItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "StringLiteralItem";
    }
};

class CPLUSPLUS_EXPORT SwitchStatementItem: public StatementItem
{
  public:
    SwitchStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
        return "SwitchStatementItem";
    }
};

class CPLUSPLUS_EXPORT TemplateArgumentListItem: public Item
{
  public:
    TemplateArgumentListItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "TemplateArgumentListItem";
    }
};

class CPLUSPLUS_EXPORT TemplateDeclarationItem: public DeclarationItem
{
  public:
    TemplateDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
        return "TemplateDeclarationItem";
    }
};

class CPLUSPLUS_EXPORT ThrowExpressionItem: public ExpressionItem
{
  public:
    ThrowExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "ThrowExpressionItem";
    }
};

class CPLUSPLUS_EXPORT TranslationUnitItem: public Item
{
  public:
    TranslationUnitItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "TranslationUnitItem";
    }
};

class CPLUSPLUS_EXPORT TryBlockStatementItem: public StatementItem
{
  public:
    TryBlockStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
        return "TryBlockStatementItem";
    }
};

class CPLUSPLUS_EXPORT CatchClauseItem: public StatementItem
{
  public:
    CatchClauseItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
        return "CatchClauseItem";
    }
};

class CPLUSPLUS_EXPORT TypeIdItem: public ExpressionItem
{
  public:
    TypeIdItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "TypeIdItem";
    }
};

class CPLUSPLUS_EXPORT TypenameTypeParameterItem: public DeclarationItem
{
  public:
    TypenameTypeParameterItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
        return "TypenameTypeParameterItem";
    }
};

class CPLUSPLUS_EXPORT TemplateTypeParameterItem: public DeclarationItem
{
  public:
    TemplateTypeParameterItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
        return "TemplateTypeParameterItem";
    }
};

class CPLUSPLUS_EXPORT UnaryExpressionItem: public ExpressionItem
{
  public:
    UnaryExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
        return "UnaryExpressionItem";
    }
};

class CPLUSPLUS_EXPORT UsingItem: public DeclarationItem
{
  public:
    UsingItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
        return "UsingItem";
    }
};

class CPLUSPLUS_EXPORT UsingDirectiveItem: public DeclarationItem
{
  public:
    UsingDirectiveItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
        return "UsingDirectiveItem";
    }
};

class CPLUSPLUS_EXPORT WhileStatementItem: public StatementItem
{
  public:
    WhileStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
        return "WhileStatementItem";
    }
};

class CPLUSPLUS_EXPORT IdentifierListItem: public Item
{
  public:
    IdentifierListItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
        return "IdentifierListItem";
    }
};

class CPLUSPLUS_EXPORT ObjCClassDeclarationItem: public DeclarationItem
{
  public:
    ObjCClassDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
        return "ObjCClassDeclarationItem";
    }
};

#endif // ASTTREEMODELITEM_H
