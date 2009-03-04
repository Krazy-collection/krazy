#include <AST.h>
#include <QtCore/QDebug>
#include <QtCore/QString>

#include "astitem.h"
#include "asttreebuilder.h"


//using namespace CPlusPlus;

TreeBuilder::TreeBuilder(Control *control):ASTVisitor(control)
{
}

TreeBuilder::~TreeBuilder()
{
}

bool TreeBuilder::preVisit(AST* ast)
{
  return true;
}

void TreeBuilder::appendItem(Item* item, AST* ast)
{
  item->setAST(ast);
  currentItem->appendChild(item);
  currentItem = item;
}


void TreeBuilder::postVisit(AST* ast)
{
  if (currentItem->parent())
    currentItem = currentItem->parent();
}

void TreeBuilder::addDummyNode(const char * str)
{
  qDebug() << str;
}

void TreeBuilder::setRootItem(Item* item)
{
  rootItem = item;
  currentItem = rootItem;
}

bool TreeBuilder::visit(TranslationUnitAST *ast)
{
  appendItem(new TranslationUnitItem(currentItem), ast );
  return true;
}

bool TreeBuilder::visit(DeclarationStatementAST *ast)
{
  appendItem(new DeclarationStatementItem(currentItem) , ast);
  return true;
}

bool TreeBuilder::visit(AccessDeclarationAST *ast)
{
  appendItem(new AccessDeclarationItem(currentItem) , ast);
  return true;
}

bool TreeBuilder::visit(SimpleDeclarationAST *ast)
{
  appendItem(new SimpleDeclarationItem(currentItem) , ast);
  return true;
}

bool TreeBuilder::visit(ArrayAccessAST *ast)
{
  appendItem(new ArrayAccessItem(currentItem) , ast);
  return true;
}

bool TreeBuilder::visit(ArrayDeclaratorAST *ast)
{
  appendItem(new ArrayDeclaratorItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ArrayInitializerAST *ast)
{
  appendItem(new ArrayInitializerItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(AsmDefinitionAST *ast)
{
  appendItem(new AsmDefinitionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(AttributeSpecifierAST *ast)
{
  appendItem(new AttributeSpecifierItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(AttributeAST *ast)
{
  appendItem(new AttributeItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(BaseSpecifierAST *ast)
{
  appendItem(new BaseSpecifierItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(BinaryExpressionAST *ast)
{
  appendItem(new BinaryExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(BoolLiteralAST *ast)
{
  appendItem(new BoolLiteralItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(BreakStatementAST *ast)
{
  appendItem(new BreakStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(CallAST *ast)
{
  appendItem(new CallItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(CaseStatementAST *ast)
{
  appendItem(new CaseStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(CastExpressionAST *ast)
{
  appendItem(new CastExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(CatchClauseAST *ast)
{
  appendItem(new CatchClauseItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ClassSpecifierAST *ast)
{
  appendItem(new ClassSpecifierItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(CompoundStatementAST *ast)
{
  appendItem(new CompoundStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ConditionAST *ast)
{
  appendItem(new ConditionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ConditionalExpressionAST *ast)
{
  appendItem(new ConditionalExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ContinueStatementAST *ast)
{
  appendItem(new ContinueStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ConversionFunctionIdAST *ast)
{
  appendItem(new ConversionFunctionIdItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(CppCastExpressionAST *ast)
{
  appendItem(new CppCastExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(CtorInitializerAST *ast)
{
  appendItem(new CtorInitializerItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(DeclaratorAST *ast)
{
  appendItem(new DeclaratorItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(DeclaratorIdAST *ast)
{
  appendItem(new DeclaratorIdItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(DeclaratorListAST *ast)
{
  appendItem(new DeclaratorListItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(DeleteExpressionAST *ast)
{
  appendItem(new DeleteExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(DestructorNameAST *ast)
{
  appendItem(new DestructorNameItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(DoStatementAST *ast)
{
  appendItem(new DoStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ElaboratedTypeSpecifierAST *ast)
{
  appendItem(new ElaboratedTypeSpecifierItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(EmptyDeclarationAST *ast)
{
  appendItem(new EmptyDeclarationItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(EnumSpecifierAST *ast)
{
  appendItem(new EnumSpecifierItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(EnumeratorAST *ast)
{
  appendItem(new EnumeratorItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ExceptionDeclarationAST *ast)
{
  appendItem(new ExceptionDeclarationItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ExceptionSpecificationAST *ast)
{
  appendItem(new ExceptionSpecificationItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ExpressionListAST *ast)
{
  appendItem(new ExpressionListItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ExpressionOrDeclarationStatementAST *ast)
{
  appendItem(new ExpressionOrDeclarationStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ExpressionStatementAST *ast)
{
  appendItem(new ExpressionStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ForStatementAST *ast)
{
  appendItem(new ForStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(FunctionDeclaratorAST *ast)
{
  appendItem(new FunctionDeclaratorItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(FunctionDefinitionAST *ast)
{
  appendItem(new FunctionDefinitionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(GotoStatementAST *ast)
{
  appendItem(new GotoStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(IfStatementAST *ast)
{
  appendItem(new IfStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(LabeledStatementAST *ast)
{
  appendItem(new LabeledStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(LinkageBodyAST *ast)
{
  appendItem(new LinkageBodyItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(LinkageSpecificationAST *ast)
{
  appendItem(new LinkageSpecificationItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(MemInitializerAST *ast)
{
  appendItem(new MemInitializerItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(MemberAccessAST *ast)
{
  appendItem(new MemberAccessItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(NamedTypeSpecifierAST *ast)
{
  appendItem(new NamedTypeSpecifierItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(NamespaceAST *ast)
{
  appendItem(new NamespaceItem(currentItem), ast);

  return true;
}

bool TreeBuilder::visit(NamespaceAliasDefinitionAST *ast)
{
  appendItem(new NamespaceAliasDefinitionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(NestedDeclaratorAST *ast)
{
  appendItem(new NestedDeclaratorItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(NestedExpressionAST *ast)
{
  appendItem(new NestedExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(NestedNameSpecifierAST *ast)
{
  appendItem(new NestedNameSpecifierItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(NewExpressionAST *ast)
{
  appendItem(new NewExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(NewInitializerAST *ast)
{
  appendItem(new NewInitializerItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(NewTypeIdAST *ast)
{
  appendItem(new NewTypeIdItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(NumericLiteralAST *ast)
{
  appendItem(new NumericLiteralItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(OperatorAST *ast)
{
  appendItem(new OperatorItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(OperatorFunctionIdAST *ast)
{
  appendItem(new OperatorFunctionIdItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ParameterDeclarationAST *ast)
{
  appendItem(new ParameterDeclarationItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ParameterDeclarationClauseAST *ast)
{
  appendItem(new ParameterDeclarationClauseItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(PtrOperatorAST *ast)
{
  appendItem(new PtrOperatorItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(PointerAST *ast)
{
  appendItem(new PointerItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(PointerToMemberAST *ast)
{
  appendItem(new PointerToMemberItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(PostIncrDecrAST *ast)
{
  appendItem(new Item(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(PostfixExpressionAST *ast)
{
  appendItem(new PostfixExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(QualifiedNameAST *ast)
{
  appendItem(new QualifiedNameItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ReferenceAST *ast)
{
  appendItem(new ReferenceItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ReturnStatementAST *ast)
{
  appendItem(new ReturnStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(SimpleNameAST *ast)
{
  appendItem(new SimpleNameItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(SimpleSpecifierAST *ast)
{
  appendItem(new SimpleSpecifierItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(SizeofExpressionAST *ast)
{
  appendItem(new SizeofExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(StringLiteralAST *ast)
{
  appendItem(new StringLiteralItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(SwitchStatementAST *ast)
{
  appendItem(new SwitchStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(TemplateArgumentListAST *ast)
{
  appendItem(new TemplateArgumentListItem(currentItem), ast);
  return true;
}


bool TreeBuilder::visit(TemplateDeclarationAST *ast)
{
  appendItem(new TemplateDeclarationItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(TemplateIdAST *ast)
{
  appendItem(new TemplateIdItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(TemplateTypeParameterAST *ast)
{
  appendItem(new TemplateTypeParameterItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ThisExpressionAST *ast)
{
  appendItem(new ThisExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(ThrowExpressionAST *ast)
{
  appendItem(new ThrowExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(TryBlockStatementAST *ast)
{
  appendItem(new TryBlockStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(TypeConstructorCallAST *ast)
{
  appendItem(new TypeConstructorCallItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(TypeIdAST *ast)
{
  appendItem(new TypeIdItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(TypeidExpressionAST *ast)
{
  appendItem(new TypeidExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(TypeofSpecifierAST *ast)
{
  appendItem(new TypeofSpecifierItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(TypenameCallExpressionAST *ast)
{
  appendItem(new TypenameCallExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(TypenameTypeParameterAST *ast)
{
  appendItem(new TypenameTypeParameterItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(UnaryExpressionAST *ast)
{
  appendItem(new UnaryExpressionItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(UsingAST *ast)
{
  appendItem(new UsingItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(UsingDirectiveAST *ast)
{
  appendItem(new UsingDirectiveItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(WhileStatementAST *ast)
{
  appendItem(new WhileStatementItem(currentItem), ast);
  return true;
}

bool TreeBuilder::visit(QtMethodAST *ast)
{
  appendItem(new QtMethodItem(currentItem), ast);
  return true;
}
