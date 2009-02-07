
#include "DefaultVisitor.h"
#include "AST.h"
#include "Token.h"

CPLUSPLUS_USE_NAMESPACE

DefaultVisitor::DefaultVisitor(Control *control)
    : ASTVisitor(control)
{ }

void DefaultVisitor::operator()(AST *ast)
{ accept(ast); }

bool DefaultVisitor::visit(AccessDeclarationAST *ast)
{
    spell(ast->access_specifier_token);
    if (ast->slots_token)
        spell(ast->slots_token);
    return false;
}

bool DefaultVisitor::visit(ArrayAccessAST *ast)
{
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(ArrayDeclaratorAST *ast)
{
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(ArrayInitializerAST *ast)
{
    for (ExpressionListAST *it = ast->expression_list; it; it = it->next)
        accept(it->expression);
    return false;
}

bool DefaultVisitor::visit(AsmDefinitionAST *ast)
{
    spell(ast->asm_token);
    for (SpecifierAST *it = ast->cv_qualifier_seq; it; it = it->next)
        accept(it);
    //out << '(';
    //out << "/* ### implement me */";
    //out << ");";
    return false;
}

bool DefaultVisitor::visit(AttributeSpecifierAST *ast)
{
    for (AttributeAST *it = ast->attributes; it; it = it->next)
        accept(it);
    return false;
}

bool DefaultVisitor::visit(AttributeAST *ast)
{
    spell(ast->identifier_token);
    if (ast->lparen_token) {
        spell(ast->tag_token);
        if (ast->expression_list) {
            for (ExpressionListAST *it = ast->expression_list; it; it = it->next)
                accept(it->expression);
        }
    }
    return false;
}

bool DefaultVisitor::visit(BaseSpecifierAST *ast)
{
    if (ast->token_virtual && ast->token_access_specifier)
        spell(ast->token_access_specifier);
    //else if (ast->token_virtual)
    else if (ast->token_access_specifier)
        spell(ast->token_access_specifier);
    accept(ast->name);
    return false;
}

bool DefaultVisitor::visit(BinaryExpressionAST *ast)
{
    accept(ast->left_expression);
    spell(ast->binary_op_token);
    accept(ast->right_expression);
    return false;
}

bool DefaultVisitor::visit(BoolLiteralAST *ast)
{
    spell(ast->token);
    return false;
}

bool DefaultVisitor::visit(BreakStatementAST *)
{
    return false;
}

bool DefaultVisitor::visit(CallAST *ast)
{
    for (ExpressionListAST *it = ast->expression_list; it; it = it->next)
        accept(it->expression);
    return false;
}

bool DefaultVisitor::visit(CaseStatementAST *ast)
{
    accept(ast->expression);
    if (! ast->statement)
        return false;

    if (ast->statement->asCompoundStatement())
        accept(ast->statement);
    else if (ast->statement->asCaseStatement() || ast->statement->asLabeledStatement())
        accept(ast->statement);
    else
        accept(ast->statement);
    return false;
}

bool DefaultVisitor::visit(CastExpressionAST *ast)
{
    accept(ast->type_id);
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(CatchClauseAST *ast)
{
    accept(ast->exception_declaration);
    accept(ast->statement);
    return false;
}

bool DefaultVisitor::visit(ClassSpecifierAST *ast)
{
    spell(ast->classkey_token);
    if (ast->attributes)
        accept(ast->attributes);
    accept(ast->name);
    if (ast->colon_token) {
        for (BaseSpecifierAST *it = ast->base_clause; it; it = it->next)
            accept(it);
    }
    if (ast->member_specifiers) {
        if (ast->member_specifiers) {
            for (DeclarationAST *it = ast->member_specifiers; it; it = it->next)
                accept(it);
        }
    }
    return false;
}

bool DefaultVisitor::visit(CompoundStatementAST *ast)
{
    if (ast->statements) {
        for (StatementAST *it = ast->statements; it; it = it->next)
            accept(it);
    }
    return false;
}

bool DefaultVisitor::visit(ConditionAST *ast)
{
    for (SpecifierAST *it = ast->type_specifier; it; it = it->next)
        accept(it);
    if (ast->declarator)
        accept(ast->declarator);
    return false;
}

bool DefaultVisitor::visit(ConditionalExpressionAST *ast)
{
    accept(ast->condition);
    accept(ast->left_expression);
    accept(ast->right_expression);
    return false;
}

bool DefaultVisitor::visit(ContinueStatementAST *)
{
    return false;
}

bool DefaultVisitor::visit(ConversionFunctionIdAST *ast)
{
    for (SpecifierAST *it = ast->type_specifier; it; it = it->next)
        accept(it);
    for (PtrOperatorAST *it = ast->ptr_operators; it; it = it->next)
        accept(it);
    return false;
}

bool DefaultVisitor::visit(CppCastExpressionAST *ast)
{
    spell(ast->cast_token);
    accept(ast->type_id);
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(CtorInitializerAST *ast)
{
    for (MemInitializerAST *it = ast->member_initializers; it; it = it->next) {
        accept(it->name);
        accept(it->expression);
    }
    return false;
}

bool DefaultVisitor::visit(DeclaratorAST *ast)
{
    for (PtrOperatorAST *it = ast->ptr_operators; it; it = it->next)
        accept(it);
    if (ast->core_declarator)
        //if (ast->ptr_operators)
        accept(ast->core_declarator);
    for (PostfixDeclaratorAST *it = ast->postfix_declarators; it; it = it->next)
        accept(it);
    for (SpecifierAST *it = ast->attributes; it; it = it->next)
        accept(it);
    if (ast->initializer)
        accept(ast->initializer);
    return false;
}

bool DefaultVisitor::visit(DeclarationStatementAST *ast)
{
    accept(ast->declaration);
    return false;
}

bool DefaultVisitor::visit(DeclaratorIdAST *ast)
{
    accept(ast->name);
    return false;
}

bool DefaultVisitor::visit(DeclaratorListAST *ast)
{
    for (DeclaratorListAST *it = ast; it; it = it->next)
        accept(it->declarator);
    return false;
}

bool DefaultVisitor::visit(DeleteExpressionAST *ast)
{
    //if (ast->scope_token)
    if (ast->expression)
        accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(DestructorNameAST *ast)
{
    spell(ast->identifier_token);
    return false;
}

bool DefaultVisitor::visit(DoStatementAST *ast)
{
    if (ast->statement)
        accept(ast->statement);
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(ElaboratedTypeSpecifierAST *ast)
{
    spell(ast->classkey_token);
    if (ast->name)
        accept(ast->name);
    return false;
}

bool DefaultVisitor::visit(EmptyDeclarationAST *)
{
    return false;
}

bool DefaultVisitor::visit(EnumSpecifierAST *ast)
{
    if (ast->name)
        accept(ast->name);
    if (ast->enumerators) {
        for (EnumeratorAST *it = ast->enumerators; it; it = it->next)
          accept(it);
    }
    return false;
}

bool DefaultVisitor::visit(EnumeratorAST *ast)
{
    spell(ast->identifier_token);
    if (ast->equal_token)
        accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(ExceptionDeclarationAST *ast)
{
    for (SpecifierAST *it = ast->type_specifier; it; it = it->next)
        accept(it);
    if (ast->declarator)
        accept(ast->declarator);
    //if (ast->dot_dot_dot_token)
    return false;
}

bool DefaultVisitor::visit(ExceptionSpecificationAST *ast)
{
    if (ast->dot_dot_dot_token) {}
    else {
        for (ExpressionListAST *it = ast->type_ids; it; it = it->next)
            accept(it->expression);
    }
    return false;
}

bool DefaultVisitor::visit(ExpressionListAST *ast)
{
    for (ExpressionListAST *it = ast; it; it = it->next)
        accept(it->expression);
    return false;
}

bool DefaultVisitor::visit(ExpressionOrDeclarationStatementAST *ast)
{
    accept(ast->declaration);
    return false;
}

bool DefaultVisitor::visit(ExpressionStatementAST *ast)
{
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(ForStatementAST *ast)
{
    accept(ast->initializer);
    accept(ast->condition);
    accept(ast->expression);
    accept(ast->statement);
    return false;
}

bool DefaultVisitor::visit(FunctionDeclaratorAST *ast)
{
    accept(ast->parameters);
    for (SpecifierAST *it = ast->cv_qualifier_seq; it; it = it->next)
        accept(it);
    if (ast->exception_specification)
        accept(ast->exception_specification);
    return false;
}

bool DefaultVisitor::visit(FunctionDefinitionAST *ast)
{
    for (SpecifierAST *it = ast->decl_specifier_seq; it; it = it->next)
        accept(it);
    if (ast->declarator)
        accept(ast->declarator);
    accept(ast->ctor_initializer);
    accept(ast->function_body);
    return false;
}

bool DefaultVisitor::visit(GotoStatementAST *ast)
{
    spell(ast->identifier_token);
    return false;
}

bool DefaultVisitor::visit(IfStatementAST *ast)
{
    accept(ast->condition);
    if (ast->statement->asCompoundStatement())
        accept(ast->statement);
    else
        accept(ast->statement);
    if (ast->else_token)
        accept(ast->else_statement);
    return false;
}

bool DefaultVisitor::visit(LabeledStatementAST *ast)
{
    spell(ast->label_token);
    accept(ast->statement);
    return false;
}

bool DefaultVisitor::visit(LinkageBodyAST *ast)
{
    if (ast->declarations) {
        for (DeclarationAST *it = ast->declarations; it; it = it->next)
            accept(it);
    }
    return false;
}

bool DefaultVisitor::visit(LinkageSpecificationAST *ast)
{
    if (ast->extern_type)
        spell(ast->extern_type);

    accept(ast->declaration);
    return false;
}

bool DefaultVisitor::visit(MemInitializerAST *ast)
{
    accept(ast->name);
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(MemberAccessAST *ast)
{
    //if (ast->template_token) {
    accept(ast->member_name);
    return false;
}

bool DefaultVisitor::visit(NamedTypeSpecifierAST *ast)
{
    accept(ast->name);
    return false;
}

bool DefaultVisitor::visit(NamespaceAST *ast)
{
    if (ast->identifier_token)
        spell(ast->identifier_token);
    for (SpecifierAST *it = ast->attributes; it; it = it->next)
        accept(it);
    accept(ast->linkage_body);
    return false;
}

bool DefaultVisitor::visit(NamespaceAliasDefinitionAST *ast)
{
    spell(ast->namespace_name);
    accept(ast->name);
    return false;
}

bool DefaultVisitor::visit(NestedDeclaratorAST *ast)
{
    accept(ast->declarator);
    return false;
}

bool DefaultVisitor::visit(NestedExpressionAST *ast)
{
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(NestedNameSpecifierAST *ast)
{
    accept(ast->class_or_namespace_name);
    //if (ast->scope_token)
    return false;
}

bool DefaultVisitor::visit(NewDeclaratorAST *ast)
{
    for (PtrOperatorAST *it = ast->ptr_operators; it; it = it->next)
        accept(it);
    if (ast->declarator)
        accept(ast->declarator);
    return false;
}

bool DefaultVisitor::visit(NewExpressionAST *ast)
{
    //if (ast->scope_token)
    if (ast->expression) {
        accept(ast->expression);
        //if (ast->type_id)
    }
    if (ast->type_id) {
        accept(ast->type_id);
        //if (ast->new_type_id)
    }
    if (ast->new_type_id) {
        accept(ast->new_type_id);
        //if (ast->new_initializer)
    }
    accept(ast->new_initializer);
    return false;
}

bool DefaultVisitor::visit(NewInitializerAST *ast)
{
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(NewTypeIdAST *ast)
{
    for (SpecifierAST *it = ast->type_specifier; it; it = it->next)
        accept(it);
    // if (ast->type_specifier)
    if (ast->new_initializer) {
        accept(ast->new_initializer);
        // if (ast->new_declarator)
    }
    accept(ast->new_declarator);
    return false;
}

bool DefaultVisitor::visit(NumericLiteralAST *ast)
{
    switch (tokenKind(ast->token)) {
    case T_CHAR_LITERAL:
        spell(ast->token);
        break;
    case T_WIDE_CHAR_LITERAL:
        spell(ast->token);
        break;

    default:
        spell(ast->token);
    }
    return false;
}

bool DefaultVisitor::visit(OperatorAST *ast)
{
    spell(ast->op_token);
    if (ast->open_token) {
        spell(ast->open_token);
        spell(ast->close_token);
    }
    return false;
}

bool DefaultVisitor::visit(OperatorFunctionIdAST *ast)
{
    accept(ast->op);
    return false;
}

bool DefaultVisitor::visit(ParameterDeclarationAST *ast)
{
    for (SpecifierAST *it = ast->type_specifier; it; it = it->next)
        accept(it);
    if (ast->declarator)
        accept(ast->declarator);
    //if (ast->equal_token)
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(ParameterDeclarationClauseAST *ast)
{
    for (DeclarationAST *it = ast->parameter_declarations; it; it = it->next)
        accept(it);
    return false;
}

bool DefaultVisitor::visit(PointerAST *ast)
{
    for (SpecifierAST *it = ast->cv_qualifier_seq; it; it = it->next)
        accept(it);
    return false;
}

bool DefaultVisitor::visit(PointerToMemberAST *ast)
{
    // if (ast->global_scope_token)
    for (NestedNameSpecifierAST *it = ast->nested_name_specifier; it; it = it->next)
        accept(it);
    for (SpecifierAST *it = ast->cv_qualifier_seq; it; it = it->next)
        accept(it);
    return false;
}

bool DefaultVisitor::visit(PostIncrDecrAST *ast)
{
    spell(ast->incr_decr_token);
    return false;
}

bool DefaultVisitor::visit(PostfixExpressionAST *ast)
{
    accept(ast->base_expression);
    for (PostfixAST *it = ast->postfix_expressions; it; it = it->next) {
        accept(it);
    }
    return false;
}

bool DefaultVisitor::visit(QualifiedNameAST *ast)
{
    // if (ast->global_scope_token)
    for (NestedNameSpecifierAST *it = ast->nested_name_specifier; it; it = it->next)
        accept(it);
    accept(ast->unqualified_name);
    return false;
}

bool DefaultVisitor::visit(ReferenceAST *)
{
    return false;
}

bool DefaultVisitor::visit(ReturnStatementAST *ast)
{
    if (ast->expression) {
        accept(ast->expression);
    }
    return false;
}

bool DefaultVisitor::visit(SimpleDeclarationAST *ast)
{
    for (SpecifierAST *it = ast->decl_specifier_seq; it; it = it->next)
        accept(it);
    if (ast->declarators) {
        //if (ast->decl_specifier_seq)

        for (DeclaratorListAST *it = ast->declarators; it; it = it->next)
            accept(it->declarator);
    }
    return false;
}

bool DefaultVisitor::visit(SimpleNameAST *ast)
{
    spell(ast->identifier_token);
    return false;
}

bool DefaultVisitor::visit(SimpleSpecifierAST *ast)
{
    spell(ast->specifier_token);
    return false;
}

bool DefaultVisitor::visit(SizeofExpressionAST *ast)
{
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(StringLiteralAST *ast)
{
    for (StringLiteralAST *it = ast; it; it = it->next) {
        if (tokenKind(ast->token) == T_STRING_LITERAL)
            spell(ast->token); 
        else
            spell(ast->token);
    }
    return false;
}

bool DefaultVisitor::visit(SwitchStatementAST *ast)
{
    accept(ast->condition);
    accept(ast->statement);
    return false;
}

bool DefaultVisitor::visit(TemplateArgumentListAST *ast)
{
    for (TemplateArgumentListAST *it = ast; it; it = it->next) {
        accept(it->template_argument);
    }
    return false;
}

bool DefaultVisitor::visit(TemplateDeclarationAST *ast)
{
    // if (ast->export_token) {
    // }
    if (ast->template_parameters) {
        for (DeclarationAST *it = ast->template_parameters; it; it = it->next)
            accept(it);
    }
    accept(ast->declaration);
    return false;
}

bool DefaultVisitor::visit(TemplateIdAST *ast)
{
    if (ast->template_arguments) {
        for (TemplateArgumentListAST *it = ast->template_arguments; it; it = it->next)
            accept(it->template_argument);
    }
    return false;
}

bool DefaultVisitor::visit(TemplateTypeParameterAST *ast)
{
    if (ast->template_parameters) {
        for (DeclarationAST *it = ast->template_parameters; it; it = it->next)
            accept(it);
    }
    accept(ast->name);
    if (ast->equal_token) {
        accept(ast->type_id);
    }
    return false;
}

bool DefaultVisitor::visit(ThisExpressionAST *)
{
    return false;
}

bool DefaultVisitor::visit(ThrowExpressionAST *ast)
{
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(TranslationUnitAST *ast)
{
    for (DeclarationAST *it = ast->declarations; it; it = it->next)
        accept(it);
    return false;
}

bool DefaultVisitor::visit(TryBlockStatementAST *ast)
{
    accept(ast->statement);
    for (CatchClauseAST *it = ast->catch_clause_seq; it; it = it->next)
        accept(it);
    return false;
}

bool DefaultVisitor::visit(TypeConstructorCallAST *ast)
{
    for (SpecifierAST *it = ast->type_specifier; it; it = it->next)
        accept(it);
    for (ExpressionListAST *it = ast->expression_list; it; it = it->next)
        accept(it->expression);
    return false;
}

bool DefaultVisitor::visit(TypeIdAST *ast)
{
    for (SpecifierAST *it = ast->type_specifier; it; it = it->next)
        accept(it);

    if (ast->type_specifier && ast->declarator)
        accept(ast->declarator);

    return false;
}

bool DefaultVisitor::visit(TypeidExpressionAST *ast)
{
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(TypeofSpecifierAST *ast)
{
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(TypenameCallExpressionAST *ast)
{
    accept(ast->name);
    for (ExpressionListAST *it = ast->expression_list; it; it = it->next)
        accept(it->expression);
    return false;
}

bool DefaultVisitor::visit(TypenameTypeParameterAST *ast)
{
    spell(ast->classkey_token);
    if (ast->name)
        accept(ast->name);
    if (ast->equal_token)
        accept(ast->type_id);
    return false;
}

bool DefaultVisitor::visit(UnaryExpressionAST *ast)
{
    spell(ast->unary_op_token);
    accept(ast->expression);
    return false;
}

bool DefaultVisitor::visit(UsingAST *ast)
{
    accept(ast->name);
    return false;
}

bool DefaultVisitor::visit(UsingDirectiveAST *ast)
{
    accept(ast->name);
    return false;
}

bool DefaultVisitor::visit(WhileStatementAST *ast)
{
    accept(ast->condition);
    if (ast->statement && ast->statement->asCompoundStatement())
        accept(ast->statement);
    else
        accept(ast->statement);
    return false;
}

bool DefaultVisitor::visit(QtMethodAST *ast)
{
    accept(ast->declarator);
    return false;
}
