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

class SpecifierItem : public Item
{
  public:
    SpecifierItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "SpecifierItem";
    }
};

class SimpleSpecifierItem : public SpecifierItem
{
  public:
    SimpleSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "SimpleSpecifierItem";
    }
};

class AttributeSpecifierItem : public SpecifierItem
{
  public:
    AttributeSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "AttributeSpecifierItem";
    }
};

class AttributeItem : public Item
{
  public:
    AttributeItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "AttributeItem";
    }
};

class TypeofSpecifierItem : public SpecifierItem
{
  public:
    TypeofSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "TypeofSpecifierItem";
    }
};

class StatementItem : public Item
{
  public:
    StatementItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "StatementItem";
    }
};

class ExpressionItem : public Item
{
  public:
    ExpressionItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "ExpressionItem";
    }
};

class DeclarationItem : public Item
{
  public:
    DeclarationItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "DeclarationItem";
    }
};

class CoreDeclaratorItem : public Item
{
  public:
    CoreDeclaratorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "CoreDeclaratorItem";
    }
};

class PostfixDeclaratorItem : public Item
{
  public:
    PostfixDeclaratorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "PostfixDeclaratorItem";
    }
};

class DeclaratorItem : public Item
{
  public:
    DeclaratorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "DeclaratorItem";
    }
};

class ExpressionListItem : public ExpressionItem
{
  public:
    ExpressionListItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "ExpressionListItem";
    }
};

class SimpleDeclarationItem : public DeclarationItem
{
  public:
    SimpleDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "SimpleDeclarationItem";
    }
};

class EmptyDeclarationItem : public DeclarationItem
{
  public:
    EmptyDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "EmptyDeclarationItem";
    }
};

class AccessDeclarationItem : public DeclarationItem
{
  public:
    AccessDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "AccessDeclarationItem";
    }
};

class AsmDefinitionItem : public DeclarationItem
{
  public:
    AsmDefinitionItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "AsmDefinitionItem";
    }
};

class BaseSpecifierItem : public Item
{
  public:
    BaseSpecifierItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "BaseSpecifierItem";
    }
};

class CompoundLiteralItem : public ExpressionItem
{
  public:
    CompoundLiteralItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "CompoundLiteralItem";
    }
};

class QtMethodItem : public ExpressionItem
{
  public:
    QtMethodItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "QtMethodItem";
    }
};

class BinaryExpressionItem : public ExpressionItem
{
  public:
    BinaryExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "BinaryExpressionItem";
    }
};

class CastExpressionItem : public ExpressionItem
{
  public:
    CastExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "CastExpressionItem";
    }
};

class ClassSpecifierItem : public SpecifierItem
{
  public:
    ClassSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "ClassSpecifierItem";
    }
};

class CaseStatementItem : public StatementItem
{
  public:
    CaseStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "CaseStatementItem";
    }
};

class CompoundStatementItem : public StatementItem
{
  public:
    CompoundStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "CompoundStatementItem";
    }
};

class ConditionItem : public ExpressionItem
{
  public:
    ConditionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "ConditionItem";
    }
};

class ConditionalExpressionItem : public ExpressionItem
{
  public:
    ConditionalExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "ConditionalExpressionItem";
    }
};

class CppCastExpressionItem : public ExpressionItem
{
  public:
    CppCastExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "CppCastExpressionItem";
    }
};

class CtorInitializerItem : public Item
{
  public:
    CtorInitializerItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "CtorInitializerItem";
    }
};

class DeclarationStatementItem : public StatementItem
{
  public:
    DeclarationStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "DeclarationStatementItem";
    }
};

class DeclaratorIdItem : public CoreDeclaratorItem
{
  public:
    DeclaratorIdItem(Item *parent = 0) : CoreDeclaratorItem(parent) {}

    virtual QVariant nameData()
    {
      return "DeclaratorIdItem";
    }
};

class NestedDeclaratorItem : public CoreDeclaratorItem
{
  public:
    NestedDeclaratorItem(Item *parent = 0) : CoreDeclaratorItem(parent) {}

    virtual QVariant nameData()
    {
      return "NestedDeclaratorItem";
    }
};

class FunctionDeclaratorItem : public PostfixDeclaratorItem
{
  public:
    FunctionDeclaratorItem(Item *parent = 0) : PostfixDeclaratorItem(parent) {}

    virtual QVariant nameData()
    {
      return "FunctionDeclaratorItem";
    }
};

class ArrayDeclaratorItem : public PostfixDeclaratorItem
{
  public:
    ArrayDeclaratorItem(Item *parent = 0) : PostfixDeclaratorItem(parent) {}

    virtual QVariant nameData()
    {
      return "ArrayDeclaratorItem";
    }
};

class DeclaratorListItem : public Item
{
  public:
    DeclaratorListItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "DeclaratorListItem";
    }
};

class DeleteExpressionItem : public ExpressionItem
{
  public:
    DeleteExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "DeleteExpressionItem";
    }
};

class DoStatementItem : public StatementItem
{
  public:
    DoStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "DoStatementItem";
    }
};

class NamedTypeSpecifierItem : public SpecifierItem
{
  public:
    NamedTypeSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "NamedTypeSpecifierItem";
    }
};

class ElaboratedTypeSpecifierItem : public SpecifierItem
{
  public:
    ElaboratedTypeSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "ElaboratedTypeSpecifierItem";
    }
};

class EnumSpecifierItem : public SpecifierItem
{
  public:
    EnumSpecifierItem(Item *parent = 0) : SpecifierItem(parent) {}

    virtual QVariant nameData()
    {
      return "EnumSpecifierItem";
    }
};

class EnumeratorItem : public Item
{
  public:
    EnumeratorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "EnumeratorItem";
    }
};

class ExceptionDeclarationItem : public DeclarationItem
{
  public:
    ExceptionDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "ExceptionDeclarationItem";
    }
};

class ExceptionSpecificationItem : public Item
{
  public:
    ExceptionSpecificationItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "ExceptionSpecificationItem";
    }
};

class ExpressionOrDeclarationStatementItem : public StatementItem
{
  public:
    ExpressionOrDeclarationStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "ExpressionOrDeclarationStatementItem";
    }
};

class ExpressionStatementItem : public StatementItem
{
  public:
    ExpressionStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "ExpressionStatementItem";
    }
};

class FunctionDefinitionItem : public DeclarationItem
{
  public:
    FunctionDefinitionItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "FunctionDefinitionItem";
    }
};

class ForStatementItem : public StatementItem
{
  public:
    ForStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "ForStatementItem";
    }
};

class IfStatementItem : public StatementItem
{
  public:
    IfStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "IfStatementItem";
    }
};

class ArrayInitializerItem : public ExpressionItem
{
  public:
    ArrayInitializerItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "ArrayInitializerItem";
    }
};

class LabeledStatementItem : public StatementItem
{
  public:
    LabeledStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "LabeledStatementItem";
    }
};

class LinkageBodyItem : public DeclarationItem
{
  public:
    LinkageBodyItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "LinkageBodyItem";
    }
};

class LinkageSpecificationItem : public DeclarationItem
{
  public:
    LinkageSpecificationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "LinkageSpecificationItem";
    }
};

class MemInitializerItem : public Item
{
  public:
    MemInitializerItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "MemInitializerItem";
    }
};

class NameItem : public ExpressionItem
{
  public:
    NameItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "NameItem";
    }
};

class NestedNameSpecifierItem : public Item
{
  public:
    NestedNameSpecifierItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "NestedNameSpecifierItem";
    }
};

class QualifiedNameItem : public NameItem
{
  public:
    QualifiedNameItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "QualifiedNameItem";
    }
};

class OperatorFunctionIdItem : public NameItem
{
  public:
    OperatorFunctionIdItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "OperatorFunctionIdItem";
    }
};

class ConversionFunctionIdItem : public NameItem
{
  public:
    ConversionFunctionIdItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "ConversionFunctionIdItem";
    }
};

class SimpleNameItem : public NameItem
{
  public:
    SimpleNameItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "SimpleNameItem";
    }
};

class DestructorNameItem : public NameItem
{
  public:
    DestructorNameItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "DestructorNameItem";
    }
};

class TemplateIdItem : public NameItem
{
  public:
    TemplateIdItem(Item *parent = 0) : NameItem(parent) {}

    virtual QVariant nameData()
    {
      return "TemplateIdItem";
    }
};

class NamespaceItem : public DeclarationItem
{
  public:
    NamespaceItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "NamespaceItem";
    }
};

class NamespaceAliasDefinitionItem : public DeclarationItem
{
  public:
    NamespaceAliasDefinitionItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "NamespaceAliasDefinitionItem";
    }
};

class NewPlacementItem : public Item
{
  public:
    NewPlacementItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "NewPlacementItem";
    }
};

class NewArrayDeclaratorItem : public Item
{
  public:
    NewArrayDeclaratorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "NewArrayDeclaratorItem";
    }
};

class NewExpressionItem : public ExpressionItem
{
  public:
    NewExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "NewExpressionItem";
    }
};

class NewInitializerItem : public Item
{
  public:
    NewInitializerItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "NewInitializerItem";
    }
};

class NewTypeIdItem : public Item
{
  public:
    NewTypeIdItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "NewTypeIdItem";
    }
};

class OperatorItem : public Item
{
  public:
    OperatorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "OperatorItem";
    }
};

class ParameterDeclarationItem : public DeclarationItem
{
  public:
    ParameterDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "ParameterDeclarationItem";
    }
};

class ParameterDeclarationClauseItem : public Item
{
  public:
    ParameterDeclarationClauseItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "ParameterDeclarationClauseItem";
    }
};

class PostfixItem : public Item
{
  public:
    PostfixItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "PostfixItem";
    }
};

class CallItem : public PostfixItem
{
  public:
    CallItem(Item *parent = 0) : PostfixItem(parent) {}

    virtual QVariant nameData()
    {
      return "CallItem";
    }
};

class ArrayAccessItem : public PostfixItem
{
  public:
    ArrayAccessItem(Item *parent = 0) : PostfixItem(parent) {}

    virtual QVariant nameData()
    {
      return "ArrayAccessItem";
    }
};

class PostIncrDecrItem : public PostfixItem
{
  public:
    PostIncrDecrItem(Item *parent = 0) : PostfixItem(parent) {}

    virtual QVariant nameData()
    {
      return "PostIncrDecrItem";
    }
};

class MemberAccessItem : public PostfixItem
{
  public:
    MemberAccessItem(Item *parent = 0) : PostfixItem(parent) {}

    virtual QVariant nameData()
    {
      return "MemberAccessItem";
    }
};

class TypeidExpressionItem : public ExpressionItem
{
  public:
    TypeidExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "TypeidExpressionItem";
    }
};

class TypenameCallExpressionItem : public ExpressionItem
{
  public:
    TypenameCallExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "TypenameCallExpressionItem";
    }
};

class TypeConstructorCallItem : public ExpressionItem
{
  public:
    TypeConstructorCallItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "TypeConstructorCallItem";
    }
};

class PostfixExpressionItem : public ExpressionItem
{
  public:
    PostfixExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "PostfixExpressionItem";
    }
};

class PtrOperatorItem : public Item
{
  public:
    PtrOperatorItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "PtrOperatorItem";
    }
};

class PointerToMemberItem : public PtrOperatorItem
{
  public:
    PointerToMemberItem(Item *parent = 0) : PtrOperatorItem(parent) {}

    virtual QVariant nameData()
    {
      return "PointerToMemberItem";
    }
};

class PointerItem : public PtrOperatorItem
{
  public:
    PointerItem(Item *parent = 0) : PtrOperatorItem(parent) {}

    virtual QVariant nameData()
    {
      return "PointerItem";
    }
};

class ReferenceItem : public PtrOperatorItem
{
  public:
    ReferenceItem(Item *parent = 0) : PtrOperatorItem(parent) {}

    virtual QVariant nameData()
    {
      return "ReferenceItem";
    }
};

class BreakStatementItem : public StatementItem
{
  public:
    BreakStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "BreakStatementItem";
    }
};

class ContinueStatementItem : public StatementItem
{
  public:
    ContinueStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "ContinueStatementItem";
    }
};

class GotoStatementItem : public StatementItem
{
  public:
    GotoStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "GotoStatementItem";
    }
};

class ReturnStatementItem : public StatementItem
{
  public:
    ReturnStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "ReturnStatementItem";
    }
};

class SizeofExpressionItem : public ExpressionItem
{
  public:
    SizeofExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "SizeofExpressionItem";
    }
};

class NumericLiteralItem : public ExpressionItem
{
  public:
    NumericLiteralItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "NumericLiteralItem";
    }
};

class BoolLiteralItem : public ExpressionItem
{
  public:
    BoolLiteralItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "BoolLiteralItem";
    }
};

class ThisExpressionItem : public ExpressionItem
{
  public:
    ThisExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "ThisExpressionItem";
    }
};

class NestedExpressionItem : public ExpressionItem
{
  public:
    NestedExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "NestedExpressionItem";
    }
};

class StringLiteralItem : public ExpressionItem
{
  public:
    StringLiteralItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "StringLiteralItem";
    }
};

class SwitchStatementItem : public StatementItem
{
  public:
    SwitchStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "SwitchStatementItem";
    }
};

class TemplateArgumentListItem : public Item
{
  public:
    TemplateArgumentListItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "TemplateArgumentListItem";
    }
};

class TemplateDeclarationItem : public DeclarationItem
{
  public:
    TemplateDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "TemplateDeclarationItem";
    }
};

class ThrowExpressionItem : public ExpressionItem
{
  public:
    ThrowExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "ThrowExpressionItem";
    }
};

class TranslationUnitItem : public Item
{
  public:
    TranslationUnitItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "TranslationUnitItem";
    }
};

class TryBlockStatementItem : public StatementItem
{
  public:
    TryBlockStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "TryBlockStatementItem";
    }
};

class CatchClauseItem : public StatementItem
{
  public:
    CatchClauseItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "CatchClauseItem";
    }
};

class TypeIdItem : public ExpressionItem
{
  public:
    TypeIdItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "TypeIdItem";
    }
};

class TypenameTypeParameterItem : public DeclarationItem
{
  public:
    TypenameTypeParameterItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "TypenameTypeParameterItem";
    }
};

class TemplateTypeParameterItem : public DeclarationItem
{
  public:
    TemplateTypeParameterItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "TemplateTypeParameterItem";
    }
};

class UnaryExpressionItem : public ExpressionItem
{
  public:
    UnaryExpressionItem(Item *parent = 0) : ExpressionItem(parent) {}

    virtual QVariant nameData()
    {
      return "UnaryExpressionItem";
    }
};

class UsingItem : public DeclarationItem
{
  public:
    UsingItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "UsingItem";
    }
};

class UsingDirectiveItem : public DeclarationItem
{
  public:
    UsingDirectiveItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "UsingDirectiveItem";
    }
};

class WhileStatementItem : public StatementItem
{
  public:
    WhileStatementItem(Item *parent = 0) : StatementItem(parent) {}

    virtual QVariant nameData()
    {
      return "WhileStatementItem";
    }
};

class IdentifierListItem : public Item
{
  public:
    IdentifierListItem(Item *parent = 0) : Item(parent) {}

    virtual QVariant nameData()
    {
      return "IdentifierListItem";
    }
};

class ObjCClassDeclarationItem : public DeclarationItem
{
  public:
    ObjCClassDeclarationItem(Item *parent = 0) : DeclarationItem(parent) {}

    virtual QVariant nameData()
    {
      return "ObjCClassDeclarationItem";
    }
};

#endif // ASTTREEMODELITEM_H
