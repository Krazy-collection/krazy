#include "treebuilder.h"

#include <AST.h>
#include <QtCore/QDebug>
#include <QtCore/QString>


#include "asttreeitem.h"


//using namespace CPlusPlus;

TreeBuilder::TreeBuilder(Control *control):ASTVisitor(control)
{
}

TreeBuilder::~TreeBuilder()
{
}

 bool TreeBuilder::preVisit(AST* ast)
{
    ASTTreeItem* tempItem = new ASTTreeItem(currentItem);
    tempItem->setAST(ast);
    currentItem->appendChild(tempItem);
    currentItem = tempItem;

    return true;
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

void TreeBuilder::setRootItem(ASTTreeItem* item)
{
    rootItem = item;
    currentItem = rootItem;
}


//
//bool TreeBuilder::visit(TranslationUnitAST *ast)
//{
//    int declNum = 0;
//
//    for (DeclarationAST *it = ast->declarations; it; it = it->next) {
//        accept(it);
//    }
//    return false;
//}
//
//bool TreeBuilder::visit(DeclarationStatementAST *ast)
//{
//    accept(ast->declaration);
//    return false;
//}
//
//bool TreeBuilder::visit(AccessDeclarationAST *ast)
//{
//    //check
//
//    return false;
//}
//
bool TreeBuilder::visit(SimpleDeclarationAST *ast)
{


    for (SpecifierAST *it = ast->decl_specifier_seq; it; it = it->next) {
        //addDummyNode("SimpleDeclarationAST::SpecifierAST");
         accept(it);
    }
    if (ast->declarators) {
        if (ast->decl_specifier_seq)
        {
            //addDummyNode("SimpleDeclarationAST::decl_specifier_seq");
        }
        for (DeclaratorListAST *it = ast->declarators; it; it = it->next) {
             //addDummyNode("SimpleDeclarationAST::DeclaratorListAST");
            accept(it->declarator);
        }
    }


    return false;

}
//
//bool TreeBuilder::visit(CtorInitializerAST *ast)
//{
//
//    for (MemInitializerAST *it = ast->member_initializers; it; it = it->next) {
//        accept(it->name);
//        //out << '(';
//        accept(it->expression);
//        //out << ')';
////        if (it->next)
////            out << ", ";
//    }
//
//    return false;
//}
//
//bool TreeBuilder::visit(ArrayAccessAST *ast)
//{
//    accept(ast->expression);
//
//    return false;
//}
//
//bool TreeBuilder::visit(ArrayDeclaratorAST *ast)
//{
//    accept(ast->expression);
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(ArrayInitializerAST *ast)
//{
//    addDummyNode("ArrayInitializerAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(AsmDefinitionAST *ast)
//{
//    addDummyNode("AsmDefinitionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(AttributeSpecifierAST *ast)
//{
//    addDummyNode("AttributeSpecifierAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(AttributeAST *ast)
//{
//    addDummyNode("AttributeAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(BaseSpecifierAST *ast)
//{
//    addDummyNode("BaseSpecifierAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(BinaryExpressionAST *ast)
//{
//
//    accept(ast->left_expression);
//    //out << ' '  << spell(ast->binary_op_token) << ' ';
//    accept(ast->right_expression);
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(BoolLiteralAST *ast)
//{
//    addDummyNode("BoolLiteralAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(BreakStatementAST *ast)
//{
//    addDummyNode("BreakStatementAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(CallAST *ast)
//{
//    addDummyNode("CallAST");
//    return false;
//}
//
//
bool TreeBuilder::visit(CaseStatementAST *ast)
{
    //out << "case ";
    accept(ast->expression);
    //out << ':';
    if (! ast->statement) {
        //newline();
        return false;
    }

    if (ast->statement->asCompoundStatement()) {
        //out << ' ';
        accept(ast->statement);
    } else if (ast->statement->asCaseStatement() || ast->statement->asLabeledStatement()) {
        //newline();
        accept(ast->statement);
    } else {
        //indent();
        //newline();
        accept(ast->statement);
        //deindent();
        //newline();
    }
    return false;
}
//
//
//bool TreeBuilder::visit(CastExpressionAST *ast)
//{
//    addDummyNode("CastExpressionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(CatchClauseAST *ast)
//{
//    addDummyNode("CatchClauseAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(ClassSpecifierAST *ast)
//{
//
//    if (ast->attributes) {
//        accept(ast->attributes);
//        qDebug() << "ClassSpecifierAST has attributes";
//
//    }
//
//    accept(ast->name);
//
//    if (ast->colon_token) {
//        qDebug() << "ClassSpecifierAST has colon token";
//        for (BaseSpecifierAST *it = ast->base_clause; it; it = it->next) {
//            accept(it);
//            if (it->next)
//               ;
//        }
//    }
//
//    if (ast->member_specifiers) {
//        //qDebug() << "ClassSpecifierAST has member specifiers";
//        if (ast->member_specifiers) {
//            for (DeclarationAST *it = ast->member_specifiers; it; it = it->next) {
//                accept(it);
//
//            }
//        }
//
//    }
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(CompoundStatementAST *ast)
//{
//     //out << '{';
//    if (ast->statements) {
//        //indent();
//        //newline();
//        for (StatementAST *it = ast->statements; it; it = it->next) {
//            accept(it);
////            if (it->next)
////                newline();
//        }
////        deindent();
////        newline();
//    }
//    //out << '}';
//    return false;
//}
//
//
//bool TreeBuilder::visit(ConditionAST *ast)
//{
//    addDummyNode("ConditionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(ConditionalExpressionAST *ast)
//{
//    addDummyNode("ConditionalExpressionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(ContinueStatementAST *ast)
//{
//    addDummyNode("ContinueStatementAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(ConversionFunctionIdAST *ast)
//{
//    addDummyNode("ConversionFunctionIdAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(CppCastExpressionAST *ast)
//{
//    addDummyNode("CppCastExpressionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(DeclaratorAST *ast)
//{
//
//    for (PtrOperatorAST *it = ast->ptr_operators; it; it = it->next) {
//        accept(it);
//    }
//    if (ast->core_declarator) {
////        if (ast->ptr_operators)
////            out << ' ';
//        accept(ast->core_declarator);
//    }
//    for (PostfixDeclaratorAST *it = ast->postfix_declarators; it; it = it->next) {
//        accept(it);
//    }
//    for (SpecifierAST *it = ast->attributes; it; it = it->next) {
//        accept(it);
////        if (it->next)
////            out << ' ';
//    }
//    if (ast->initializer) {
////        out << ' ';
////        out << '=';
////        out << ' ';
//        accept(ast->initializer);
//    }
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(DeclaratorIdAST *ast)
//{
//    accept(ast->name);
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(DeclaratorListAST *ast)
//{
//    addDummyNode("DeclaratorListAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(DeleteExpressionAST *ast)
//{
//    addDummyNode("DeleteExpressionAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(DestructorNameAST *ast)
//{
//    addDummyNode("DestructorNameAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(DoStatementAST *ast)
//{
//    addDummyNode("DoStatementAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(ElaboratedTypeSpecifierAST *ast)
//{
//    addDummyNode("ElaboratedTypeSpecifierAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(EmptyDeclarationAST *ast)
//{
//    addDummyNode("EmptyDeclarationAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(EnumSpecifierAST *ast)
//{
//    addDummyNode("EnumSpecifierAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(EnumeratorAST *ast)
//{
//    addDummyNode("EnumeratorAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(ExceptionDeclarationAST *ast)
//{
//    addDummyNode("ExceptionDeclarationAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(ExceptionSpecificationAST *ast)
//{
//    addDummyNode("ExceptionSpecificationAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(ExpressionListAST *ast)
//{
//    addDummyNode("ExpressionListAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(ExpressionOrDeclarationStatementAST *ast)
//{
//    accept(ast->declaration);
//    return true;
//}
//
//
//bool TreeBuilder::visit(ExpressionStatementAST *ast)
//{
//    //accept(ast->expression);
//    //out << ';';
//    return true;
//}
//
//
//bool TreeBuilder::visit(ForStatementAST *ast)
//{
//    addDummyNode("ForStatementAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(FunctionDeclaratorAST *ast)
//{
//    ASTTreeItem* tempItem = new ASTTreeItem(currentItem);
//    tempItem->setAST(ast);
//
//    currentItem->appendChild(tempItem);
//
//    currentItem = tempItem;
//    accept(ast->parameters);
//    //out << ')';
//    for (SpecifierAST *it = ast->cv_qualifier_seq; it; it = it->next) {
//        //out << ' ';
//        accept(it);
//    }
//    if (ast->exception_specification) {
//        //out << ' ';
//        accept(ast->exception_specification);
//    }
//
//    if (tempItem->parent())
//        currentItem = tempItem->parent();
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(FunctionDefinitionAST *ast)
//{
//    ASTTreeItem* tempItem = new ASTTreeItem(currentItem);
//    tempItem->setAST(ast);
//
//    currentItem->appendChild(tempItem);
//
//    currentItem = tempItem;
//
//    for (SpecifierAST *it = ast->decl_specifier_seq; it; it = it->next) {
//        accept(it);
////        if (it->next)
////            out << ' ';
//    }
//    if (ast->declarator) {
////        if (ast->decl_specifier_seq)
////            out << ' ';
//        accept(ast->declarator);
//    }
//    accept(ast->ctor_initializer);
//    //newline();
//    accept(ast->function_body);
//   /* if (ast->next)
//        newline(); */// one extra line after the function definiton.
//
//    if (tempItem->parent())
//        currentItem = tempItem->parent();
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(GotoStatementAST *ast)
//{
//    addDummyNode("GotoStatementAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(IfStatementAST *ast)
//{
//    addDummyNode("IfStatementAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(LabeledStatementAST *ast)
//{
//    addDummyNode("LabeledStatementAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(LinkageBodyAST *ast)
//{
//    addDummyNode("LinkageBodyAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(LinkageSpecificationAST *ast)
//{
//    addDummyNode("LinkageSpecificationAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(MemInitializerAST *ast)
//{
//    addDummyNode("MemInitializerAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(MemberAccessAST *ast)
//{
//
//
//    accept(ast->member_name);
//
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(NamedTypeSpecifierAST *ast)
//{
//    accept(ast->name);
//    return false;
//}
//
//
//bool TreeBuilder::visit(NamespaceAST *ast)
//{
//    addDummyNode("NamespaceAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(NamespaceAliasDefinitionAST *ast)
//{
//    addDummyNode("NamespaceAliasDefinitionAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(NestedDeclaratorAST *ast)
//{
//    addDummyNode("NestedDeclaratorAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(NestedExpressionAST *ast)
//{
//    addDummyNode("NestedExpressionAST");
//    return true;
//}
//
//
//bool TreeBuilder::visit(NestedNameSpecifierAST *ast)
//{
//    accept(ast->class_or_namespace_name);
////    if (ast->scope_token)
////        out << "::";
//    return false;
//}
//
//
//
//bool TreeBuilder::visit(NewExpressionAST *ast)
//{
//
//    if (ast->expression) {
//        accept(ast->expression);
////        if (ast->type_id)
////            out << ' ';
//    }
//    if (ast->type_id) {
//        accept(ast->type_id);
////        if (ast->new_type_id)
////            out << ' ';
//    }
//    if (ast->new_type_id) {
//        accept(ast->new_type_id);
////        if (ast->new_initializer)
////            out << ' ';
//    }
//
//    accept(ast->new_initializer);
//
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(NewInitializerAST *ast)
//{
//
//    //out << '(';
//    accept(ast->expression);
//    //out << ')';
//
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(NewTypeIdAST *ast)
//{
//
//    for (SpecifierAST *it = ast->type_specifier; it; it = it->next) {
//        accept(it);
////        if (it->next)
////            out << ' ';
//    }
////    if (ast->type_specifier)
////        out << ' ';
//    if (ast->new_initializer) {
//        accept(ast->new_initializer);
////        if (ast->new_declarator)
////            out << ' ';
//    }
//    accept(ast->new_declarator);
//
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(NumericLiteralAST *ast)
//{
//
//
//    return true;
//}
//
//
//bool TreeBuilder::visit(OperatorAST *ast)
//{
//    addDummyNode("OperatorAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(OperatorFunctionIdAST *ast)
//{
//    addDummyNode("OperatorFunctionIdAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(ParameterDeclarationAST *ast)
//{
//    ASTTreeItem* tempItem = new ASTTreeItem(currentItem);
//    tempItem->setAST(ast);
//
//    currentItem->appendChild(tempItem);
//
//    currentItem = tempItem;
//
//    for (SpecifierAST *it = ast->type_specifier; it; it = it->next)
//    {
//        accept(it);
//    }
//
//    if (ast->declarator) {
//        accept(ast->declarator);
//    }
//    if (ast->equal_token) {
////        out << ' ';
////        out << '=';
////        out << ' ';
//    }
//
//    accept(ast->expression);
//
//    if (tempItem->parent())
//       currentItem = tempItem->parent();
//    return false;
//}
//
////nechat
bool TreeBuilder::visit(ParameterDeclarationClauseAST *ast)
{

    if (!ast->parameter_declarations)
        return false;

    ASTTreeItem* tempItem = new ASTTreeItem(currentItem);
    tempItem->setAST(ast);

    currentItem->appendChild(tempItem);

    currentItem = tempItem;

   for (DeclarationAST *it = ast->parameter_declarations; it; it = it->next) {
        accept(it);

    }

   if (tempItem->parent())
       currentItem = tempItem->parent();

    return false;
}
//
//bool TreeBuilder::visit(PtrOperatorAST *ast)
//{
//qDebug() << "TreeBuilder::visit(PtrOperatorAST *ast)";
//    return true;
//}
//
//
//bool TreeBuilder::visit(PointerAST *ast)
//{
//    //qDebug() << "TreeBuilder::visit(PointerAST *ast)";
//    //out << '*';
////    if (!ast->cv_qualifier_seq)
////        return false;
//
//    ASTTreeItem* tempItem = new ASTTreeItem(currentItem);
//    tempItem->setAST(ast);
//
//    currentItem->appendChild(tempItem);
//
//    currentItem = tempItem;
//
//    for (SpecifierAST *it = ast->cv_qualifier_seq; it; it = it->next) {
//        accept(it);
//    }
//
//    if (tempItem->parent())
//       currentItem = tempItem->parent();
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(PointerToMemberAST *ast)
//{
//    addDummyNode("PointerToMemberAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(PostIncrDecrAST *ast)
//{
//    addDummyNode("PostIncrDecrAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(PostfixExpressionAST *ast)
//{
//    ASTTreeItem* tempItem = new ASTTreeItem(currentItem);
//    tempItem->setAST(ast);
//    currentItem->appendChild(tempItem);
//    currentItem = tempItem;
//
//    accept(ast->base_expression);
//    for (PostfixAST *it = ast->postfix_expressions; it; it = it->next) {
//        accept(it);
//    }
//
//    if (tempItem->parent())
//        currentItem = tempItem->parent();
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(QualifiedNameAST *ast)
//{
//    ASTTreeItem* tempItem = new ASTTreeItem(currentItem);
//    tempItem->setAST(ast);
//
//    currentItem->appendChild(tempItem);
//
//    currentItem = tempItem;
//
////     if (ast->global_scope_token)
////        out << "::";
//    for (NestedNameSpecifierAST *it = ast->nested_name_specifier; it; it = it->next) {
//        accept(it);
//    }
//    accept(ast->unqualified_name);
//
//    if (tempItem->parent())
//        currentItem = tempItem->parent();
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(ReferenceAST *ast)
//{
//    addDummyNode("ReferenceAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(ReturnStatementAST *ast)
//{
//    addDummyNode("ReturnStatementAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(SimpleNameAST *ast)
//{
//    ASTTreeItem* tempItem = new ASTTreeItem(currentItem);
//    tempItem->setAST(ast);
//    tempItem->setItemType(ASTTreeItem::SimpleNameItem);
//    currentItem->appendChild(tempItem);
//
//    return false;
//}
//
//
//bool TreeBuilder::visit(SimpleSpecifierAST *ast)
//{
//    ASTTreeItem* tempItem = new ASTTreeItem(currentItem);
//    tempItem->setAST(ast);
//    tempItem->setItemType(ASTTreeItem::SimpleSpecifierItem);
//    currentItem->appendChild(tempItem);
//    return false;
//}
//
//
//bool TreeBuilder::visit(SizeofExpressionAST *ast)
//{
//    addDummyNode("SizeofExpressionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(StringLiteralAST *ast)
//{
//    addDummyNode("StringLiteralAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(SwitchStatementAST *ast)
//{
//    addDummyNode("SwitchStatementAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TemplateArgumentListAST *ast)
//{
//    addDummyNode("TemplateArgumentListAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TemplateDeclarationAST *ast)
//{
//    addDummyNode("TemplateDeclarationAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TemplateIdAST *ast)
//{
//    addDummyNode("TemplateIdAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TemplateTypeParameterAST *ast)
//{
//    addDummyNode("TemplateTypeParameterAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(ThisExpressionAST *ast)
//{
//    addDummyNode("ThisExpressionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(ThrowExpressionAST *ast)
//{
//    addDummyNode("ThrowExpressionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TryBlockStatementAST *ast)
//{
//    addDummyNode("TryBlockStatementAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TypeConstructorCallAST *ast)
//{
//    addDummyNode("TypeConstructorCallAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TypeIdAST *ast)
//{
//    addDummyNode("TypeIdAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TypeidExpressionAST *ast)
//{
//    addDummyNode("TypeidExpressionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TypeofSpecifierAST *ast)
//{
//    addDummyNode("TypeofSpecifierAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TypenameCallExpressionAST *ast)
//{
//    addDummyNode("TypenameCallExpressionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(TypenameTypeParameterAST *ast)
//{
//    addDummyNode("TypenameTypeParameterAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(UnaryExpressionAST *ast)
//{
//    addDummyNode("UnaryExpressionAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(UsingAST *ast)
//{
//    addDummyNode("UsingAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(UsingDirectiveAST *ast)
//{
//    addDummyNode("UsingDirectiveAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(WhileStatementAST *ast)
//{
//    addDummyNode("WhileStatementAST");
//    return false;
//}
//
//
//bool TreeBuilder::visit(QtMethodAST *ast)
//{
//    addDummyNode("QtMethodAST");
//    return false;
//}
//
