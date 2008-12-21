/* This file is part of KDevelop
    Copyright 2002-2005 Roberto Raggi <roberto@kdevelop.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef AST_H
#define AST_H

#include "memorypool.h"
#include "listnode.h"

#define DECLARE_AST_NODE(k) \
    enum { __node_kind = Kind_##k };

class ParseSession;
namespace KDevelop
{
class DUContext;
}
struct AccessSpecifierAST;
struct AsmDefinitionAST;
struct BaseClauseAST;
struct BaseSpecifierAST;
struct BinaryExpressionAST;
struct CastExpressionAST;
struct ClassMemberAccessAST;
struct ClassSpecifierAST;
struct CompoundStatementAST;
struct ConditionAST;
struct ConditionalExpressionAST;
struct CppCastExpressionAST;
struct CtorInitializerAST;
struct DeclarationAST;
struct DeclarationStatementAST;
struct DeclaratorAST;
struct DeleteExpressionAST;
struct DoStatementAST;
struct Elaborated;
struct EnumSpecifierAST;
struct EnumeratorAST;
struct ExceptionSpecificationAST;
struct ExpressionAST;
struct ExpressionOrDeclarationStatementAST;
struct ExpressionStatementAST;
struct ForStatementAST;
struct FunctionCallAST;
struct FunctionDefinitionAST;
struct IfStatementAST;
struct IncrDecrExpressionAST;
struct InitDeclaratorAST;
struct InitializerAST;
struct InitializerClauseAST;
struct LabeledStatementAST;
struct LinkageBodyAST;
struct LinkageSpecificationAST;
struct MemInitializerAST;
struct NameAST;
struct NamespaceAST;
struct NamespaceAliasDefinitionAST;
struct NewDeclaratorAST;
struct NewExpressionAST;
struct NewInitializerAST;
struct NewTypeIdAST;
struct OperatorAST;
struct OperatorFunctionIdAST;
struct ParameterDeclarationAST;
struct ParameterDeclarationClauseAST;
struct PostfixExpressionAST;
struct PrimaryExpressionAST;
struct PtrOperatorAST;
struct PtrToMemberAST;
struct ReturnStatementAST;
struct SimpleDeclarationAST;
struct SimpleTypeSpecifierAST;
struct SizeofExpressionAST;
struct StatementAST;
struct StringLiteralAST;
struct SubscriptExpressionAST;
struct SwitchStatementAST;
struct TemplateArgumentAST;
struct TemplateDeclarationAST;
struct TemplateParameterAST;
struct ThrowExpressionAST;
struct TranslationUnitAST;
struct TryBlockStatementAST;
struct CatchStatementAST;
struct TypeIdAST;
struct TypeIdentificationAST;
struct TypeParameterAST;
struct TypeSpecifierAST;
struct TypedefAST;
struct UnaryExpressionAST;
struct UnqualifiedNameAST;
struct UsingAST;
struct UsingDirectiveAST;
struct WhileStatementAST;
struct WinDeclSpecAST;

struct AST
{///@warning When adding new nodes here, also modify the names[] array in dumptree.cpp
  enum NODE_KIND
    {
      Kind_UNKNOWN = 0,

      Kind_AccessSpecifier,                     // 1
      Kind_AsmDefinition,                       // 2
      Kind_BaseClause,                          // 3
      Kind_BaseSpecifier,                       // 4
      Kind_BinaryExpression,                    // 5
      Kind_CastExpression,                      // 6
      Kind_ClassMemberAccess,                   // 7
      Kind_ClassSpecifier,                      // 8
      Kind_CompoundStatement,                   // 9
      Kind_Condition,                           // 10
      Kind_ConditionalExpression,               // 11
      Kind_CppCastExpression,                   // 12
      Kind_CtorInitializer,                     // 13
      Kind_DeclarationStatement,                // 14
      Kind_Declarator,                          // 15
      Kind_DeleteExpression,                    // 16
      Kind_DoStatement,                         // 17
      Kind_ElaboratedTypeSpecifier,             // 18
      Kind_EnumSpecifier,                       // 19
      Kind_Enumerator,                          // 20
      Kind_ExceptionSpecification,              // 21
      Kind_ExpressionOrDeclarationStatement,    // 22
      Kind_ExpressionStatement,                 // 23
      Kind_ForStatement,                        // 24
      Kind_FunctionCall,                        // 25
      Kind_FunctionDefinition,                  // 26
      Kind_IfStatement,                         // 27
      Kind_IncrDecrExpression,                  // 28
      Kind_InitDeclarator,                      // 29
      Kind_Initializer,                         // 30
      Kind_InitializerClause,                   // 31
      Kind_LabeledStatement,                    // 32
      Kind_LinkageBody,                         // 33
      Kind_LinkageSpecification,                // 34
      Kind_MemInitializer,                      // 35
      Kind_Name,                                // 36
      Kind_Namespace,                           // 37
      Kind_NamespaceAliasDefinition,            // 38
      Kind_NewDeclarator,                       // 39
      Kind_NewExpression,                       // 40
      Kind_NewInitializer,                      // 41
      Kind_NewTypeId,                           // 42
      Kind_Operator,                            // 43
      Kind_OperatorFunctionId,                  // 44
      Kind_ParameterDeclaration,                // 45
      Kind_ParameterDeclarationClause,          // 46
      Kind_PostfixExpression,                   // 47
      Kind_PrimaryExpression,                   // 48
      Kind_PtrOperator,                         // 49
      Kind_PtrToMember,                         // 50
      Kind_ReturnStatement,                     // 51
      Kind_SimpleDeclaration,                   // 52
      Kind_SimpleTypeSpecifier,                 // 53
      Kind_SizeofExpression,                    // 54
      Kind_StringLiteral,                       // 55
      Kind_SubscriptExpression,                 // 56
      Kind_SwitchStatement,                     // 57
      Kind_TemplateArgument,                    // 58
      Kind_TemplateDeclaration,                 // 59
      Kind_TemplateParameter,                   // 60
      Kind_ThrowExpression,                     // 61
      Kind_TranslationUnit,                     // 62
      Kind_TryBlockStatement,                   // 63
      Kind_CatchStatement,                      // 64
      Kind_TypeId,                              // 65
      Kind_TypeIdentification,                  // 66
      Kind_TypeParameter,                       // 67
      Kind_Typedef,                             // 68
      Kind_UnaryExpression,                     // 69
      Kind_UnqualifiedName,                     // 70
      Kind_Using,                               // 71
      Kind_UsingDirective,                      // 72
      Kind_WhileStatement,                      // 73
      Kind_WinDeclSpec,                         // 74
      Kind_Comment,                             // 75
      Kind_JumpStatement,                       // 76
      Kind_SignalSlotExpression,                     // 77
      NODE_KIND_COUNT
    };

  int kind;

  std::size_t start_token;
  std::size_t end_token;

  /** Context opened by this AST
    *This is usually not filled for all AST's, only for those that open a new context
    */
  KDevelop::DUContext* ducontext;
};

struct CommentAST {
  const ListNode<std::size_t> *comments; //A list of comment-tokens
};

struct TypeSpecifierAST: public AST
{
  const ListNode<std::size_t> *cv; // const or volatile tokens
};

struct StatementAST: public AST
{
};

struct ExpressionAST: public AST
{
};

struct DeclarationAST: public AST, public CommentAST
{
};

struct AccessSpecifierAST: public DeclarationAST
{
  DECLARE_AST_NODE(AccessSpecifier)

  const ListNode<std::size_t> *specs;
};

struct AsmDefinitionAST: public DeclarationAST
{
  DECLARE_AST_NODE(AsmDefinition)

  const ListNode<std::size_t> *cv;
};

struct BaseClauseAST: public AST // ### kill me
{
  DECLARE_AST_NODE(BaseClause)

  const ListNode<BaseSpecifierAST*> *base_specifiers;
};

struct BaseSpecifierAST: public AST
{
  DECLARE_AST_NODE(BaseSpecifier)

  std::size_t virt;
  std::size_t access_specifier;
  NameAST *name;
};

struct BinaryExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(BinaryExpression)

  std::size_t op; //Index of the token that describes the operator
  ExpressionAST *left_expression;
  ExpressionAST *right_expression;
};

///An expression used to do more detailed processing of SIGNAL(..) and SLOT(..) specifications
struct SignalSlotExpressionAST : public ExpressionAST {
  DECLARE_AST_NODE(SignalSlotExpression)
  //The unqualified name also contains the argument types in its template-arguments
  UnqualifiedNameAST *name;
};

struct CastExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(CastExpression)

  TypeIdAST *type_id;
  ExpressionAST *expression;
};

struct ClassMemberAccessAST: public ExpressionAST
{
  DECLARE_AST_NODE(ClassMemberAccess)

  std::size_t op; //Index of the token that describes the operator
  NameAST *name;
};

struct ClassSpecifierAST: public TypeSpecifierAST
{
  DECLARE_AST_NODE(ClassSpecifier)

  WinDeclSpecAST *win_decl_specifiers;
  std::size_t class_key;
  NameAST *name;
  BaseClauseAST *base_clause;
  const ListNode<DeclarationAST*> *member_specs;
};

struct CompoundStatementAST: public StatementAST
{
  DECLARE_AST_NODE(CompoundStatement)

  const ListNode<StatementAST*> *statements;
};

struct ConditionAST: public AST
{
  DECLARE_AST_NODE(Condition)

  TypeSpecifierAST *type_specifier;
  DeclaratorAST *declarator;
  ExpressionAST *expression;
};

struct ConditionalExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(ConditionalExpression)

  ExpressionAST *condition;
  ExpressionAST *left_expression;
  ExpressionAST *right_expression;
};

/**
 * type_id is the type that should be casted to
 * expression is the expression casted from
 * sub_expressions is a list of post-fix expressions, see PostfixExpressionAST
 */
struct CppCastExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(CppCastExpression)

  std::size_t op; //Index of the token that describes the operator
  TypeIdAST *type_id;
  ExpressionAST *expression;
  const ListNode<ExpressionAST*> *sub_expressions;
};

struct CtorInitializerAST: public AST
{
  DECLARE_AST_NODE(CtorInitializer)

  std::size_t colon;
  const ListNode<MemInitializerAST*> *member_initializers;
};

struct DeclarationStatementAST: public StatementAST
{
  DECLARE_AST_NODE(DeclarationStatement)

  DeclarationAST *declaration;
};

struct DeclaratorAST: public AST
{
  DECLARE_AST_NODE(Declarator)

  const ListNode<PtrOperatorAST*> *ptr_ops;
  DeclaratorAST *sub_declarator;
  NameAST *id;
  ExpressionAST *bit_expression;
  const ListNode<ExpressionAST*> *array_dimensions;
  ParameterDeclarationClauseAST *parameter_declaration_clause;
  const ListNode<std::size_t> *fun_cv;
  ExceptionSpecificationAST *exception_spec;
};

struct DeleteExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(DeleteExpression)

  std::size_t scope_token;
  std::size_t delete_token;
  std::size_t lbracket_token;
  std::size_t rbracket_token;
  ExpressionAST *expression;
};

struct DoStatementAST: public StatementAST
{
  DECLARE_AST_NODE(DoStatement)

  StatementAST *statement;
  ExpressionAST *expression;
};

struct ElaboratedTypeSpecifierAST: public TypeSpecifierAST
{
  DECLARE_AST_NODE(ElaboratedTypeSpecifier)

  std::size_t type;
  NameAST *name;
};

struct EnumSpecifierAST: public TypeSpecifierAST
{
  DECLARE_AST_NODE(EnumSpecifier)

  NameAST *name;
  const ListNode<EnumeratorAST*> *enumerators;
};

struct EnumeratorAST: public AST, public  CommentAST
{
  DECLARE_AST_NODE(Enumerator)

  std::size_t id;
  ExpressionAST *expression;
};

struct ExceptionSpecificationAST: public AST
{
  DECLARE_AST_NODE(ExceptionSpecification)

  std::size_t ellipsis;
  const ListNode<TypeIdAST*> *type_ids;
};

struct ExpressionOrDeclarationStatementAST: public StatementAST
{
  DECLARE_AST_NODE(ExpressionOrDeclarationStatement)

  StatementAST *expression;
  StatementAST *declaration;

  // This was not originally part of the AST - added by the context visitor
  bool expressionChosen;
};

///An expression terminated by a semicolon or similar
struct ExpressionStatementAST: public StatementAST
{
  DECLARE_AST_NODE(ExpressionStatement)

  ExpressionAST *expression;
};

struct FunctionCallAST: public ExpressionAST
{
  DECLARE_AST_NODE(FunctionCall)

  ExpressionAST *arguments;
};

struct FunctionDefinitionAST: public DeclarationAST
{
  DECLARE_AST_NODE(FunctionDefinition)

  const ListNode<std::size_t> *storage_specifiers;
  const ListNode<std::size_t> *function_specifiers;
  TypeSpecifierAST *type_specifier;
  InitDeclaratorAST *init_declarator;
  StatementAST *function_body;
  WinDeclSpecAST *win_decl_specifiers;
  CtorInitializerAST *constructor_initializers;
};

struct ForStatementAST: public StatementAST
{
  DECLARE_AST_NODE(ForStatement)

  StatementAST *init_statement;
  ConditionAST *condition;
  ExpressionAST *expression;
  StatementAST *statement;
};

struct IfStatementAST: public StatementAST
{
  DECLARE_AST_NODE(IfStatement)

  ConditionAST *condition;
  StatementAST *statement;
  StatementAST *else_statement;
};

struct IncrDecrExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(IncrDecrExpression)

  std::size_t op; //Index of the token that describes the operator
};

struct InitDeclaratorAST: public AST
{
  DECLARE_AST_NODE(InitDeclarator)

  DeclaratorAST *declarator;
  InitializerAST *initializer;
};

struct InitializerAST: public AST
{
  DECLARE_AST_NODE(Initializer)

  InitializerClauseAST *initializer_clause;
  ExpressionAST *expression;
};

struct InitializerClauseAST: public AST
{
  DECLARE_AST_NODE(InitializerClause)

  // either 'expression' or 'initializer_list' or neither are used.
  // neither are used when the clause represents the empty initializer "{}"

  // assignment expression
  ExpressionAST *expression;
  const ListNode<InitializerClauseAST*> *initializer_list;
};

struct LabeledStatementAST: public StatementAST
{
  DECLARE_AST_NODE(LabeledStatement)

  //The constant label expression
  ExpressionAST *expression;
  StatementAST* statement;
};

struct LinkageBodyAST: public AST
{
  DECLARE_AST_NODE(LinkageBody)

  const ListNode<DeclarationAST*> *declarations;
};

struct LinkageSpecificationAST: public DeclarationAST
{
  DECLARE_AST_NODE(LinkageSpecification)

  std::size_t extern_type;
  LinkageBodyAST *linkage_body;
  DeclarationAST *declaration;
};

struct MemInitializerAST: public AST
{
  DECLARE_AST_NODE(MemInitializer)

  NameAST *initializer_id;
  ExpressionAST *expression;
};

struct NameAST: public AST
{
  DECLARE_AST_NODE(Name)

  bool global;
  const ListNode<UnqualifiedNameAST*> *qualified_names;
  UnqualifiedNameAST *unqualified_name;
};

struct NamespaceAST: public DeclarationAST
{
  DECLARE_AST_NODE(Namespace)

  std::size_t namespace_name;
  LinkageBodyAST *linkage_body;
};

struct NamespaceAliasDefinitionAST: public DeclarationAST
{
  DECLARE_AST_NODE(NamespaceAliasDefinition)

  std::size_t namespace_name;
  NameAST *alias_name;
};

struct NewDeclaratorAST: public AST
{
  DECLARE_AST_NODE(NewDeclarator)

  PtrOperatorAST *ptr_op;
  NewDeclaratorAST *sub_declarator;
  const ListNode<ExpressionAST*> *expressions;
};

struct NewExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(NewExpression)

  std::size_t scope_token;
  std::size_t new_token;
  ExpressionAST *expression;
  TypeIdAST *type_id;
  NewTypeIdAST *new_type_id;
  NewInitializerAST *new_initializer;
};

struct NewInitializerAST: public AST
{
  DECLARE_AST_NODE(NewInitializer)

  ExpressionAST *expression;
};

struct NewTypeIdAST: public AST
{
  DECLARE_AST_NODE(NewTypeId)

  TypeSpecifierAST *type_specifier;
  NewInitializerAST *new_initializer;
  NewDeclaratorAST *new_declarator;
};

struct OperatorAST: public AST
{
  DECLARE_AST_NODE(Operator)

  std::size_t op; //Index of the token that describes the operator
  std::size_t open;
  std::size_t close;
};

struct OperatorFunctionIdAST: public AST
{
  DECLARE_AST_NODE(OperatorFunctionId)

  OperatorAST *op;
  TypeSpecifierAST *type_specifier;
  const ListNode<PtrOperatorAST*> *ptr_ops;
};

struct ParameterDeclarationAST: public AST
{
  DECLARE_AST_NODE(ParameterDeclaration)

  TypeSpecifierAST *type_specifier;
  DeclaratorAST *declarator;
  ExpressionAST *expression;
};

struct ParameterDeclarationClauseAST: public AST
{
  DECLARE_AST_NODE(ParameterDeclarationClause)

  const ListNode<ParameterDeclarationAST*> *parameter_declarations;
  std::size_t ellipsis;
};

/**
 * A post-fix expression is an expression that consists of one primary expression and multiple sub-expressions that are evaluated from
 * left to right, each  sub-expression based on the previous expression.
 *
 *
 * Examples:
 * "a->b"  : "a" is the primary expression, "->b" is a sub-expression
 * "a->b(5,3)" : "a" is the primary expression, "->b" is a sub-expression, and "(5,3)" is a sub-expression
 **/
struct PostfixExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(PostfixExpression)

  TypeSpecifierAST *type_specifier;
  ExpressionAST *expression;
  const ListNode<ExpressionAST*> *sub_expressions;
};

struct PrimaryExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(PrimaryExpression)

  StringLiteralAST *literal;
  std::size_t token;
  StatementAST *expression_statement;
  ExpressionAST *sub_expression;
  NameAST *name;
};

struct PtrOperatorAST: public AST
{
  DECLARE_AST_NODE(PtrOperator)

  const ListNode<std::size_t> *cv;
  std::size_t op; //Index of the token that describes the operator. Is zero when mem_ptr is non-zero.
  PtrToMemberAST *mem_ptr;
};

struct PtrToMemberAST: public AST
{
  DECLARE_AST_NODE(PtrToMember)
};

struct JumpStatementAST : public StatementAST
{
  DECLARE_AST_NODE(JumpStatement)

  // index of operator token which describes the jump, one of
  // 'break', 'continue' or 'goto.  Return statements are handled by
  // ReturnStatementAST
  std::size_t op;
  // identifier for 'goto' statements
  std::size_t identifier;
};

struct ReturnStatementAST: public StatementAST
{
  DECLARE_AST_NODE(ReturnStatement)

  ExpressionAST *expression;
};

struct SimpleDeclarationAST: public DeclarationAST
{
  DECLARE_AST_NODE(SimpleDeclaration)

  const ListNode<std::size_t> *storage_specifiers;
  const ListNode<std::size_t> *function_specifiers;
  TypeSpecifierAST *type_specifier;
  const ListNode<InitDeclaratorAST*> *init_declarators;
  WinDeclSpecAST *win_decl_specifiers;
};

struct SimpleTypeSpecifierAST: public TypeSpecifierAST
{
  DECLARE_AST_NODE(SimpleTypeSpecifier)

  const ListNode<std::size_t> *integrals;
  std::size_t type_of;
  TypeIdAST *type_id;
  ExpressionAST *expression;
  NameAST *name;
};

struct SizeofExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(SizeofExpression)

  std::size_t sizeof_token;
  TypeIdAST *type_id;
  ExpressionAST *expression;
};

struct StringLiteralAST: public AST
{
  DECLARE_AST_NODE(StringLiteral)

  const ListNode<std::size_t> *literals;
};

/// operator []
struct SubscriptExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(SubscriptExpression)

  ExpressionAST *subscript;
};

struct SwitchStatementAST: public StatementAST
{
  DECLARE_AST_NODE(SwitchStatement)

  ConditionAST *condition;
  StatementAST *statement;
};

struct TemplateArgumentAST: public AST
{
  DECLARE_AST_NODE(TemplateArgument)

  TypeIdAST *type_id;
  ExpressionAST *expression;
};

struct TemplateDeclarationAST: public DeclarationAST
{
  DECLARE_AST_NODE(TemplateDeclaration)

  std::size_t exported;
  const ListNode<TemplateParameterAST*> *template_parameters;
  DeclarationAST* declaration;
};

struct TemplateParameterAST: public AST
{
  DECLARE_AST_NODE(TemplateParameter)

  TypeParameterAST *type_parameter;
  ParameterDeclarationAST *parameter_declaration; //Default-parameter
};

struct ThrowExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(ThrowExpression)

  std::size_t throw_token;
  ExpressionAST *expression;
};

struct TranslationUnitAST: public AST, public CommentAST
{
  DECLARE_AST_NODE(TranslationUnit)

  const ListNode<DeclarationAST*> *declarations;

  // Note: non AST related, saves parsing session...
  ParseSession* session;
};

struct TryBlockStatementAST: public StatementAST
{
  DECLARE_AST_NODE(TryBlockStatement)

  StatementAST* try_block;
  const ListNode<CatchStatementAST*> *catch_blocks;
};

struct CatchStatementAST: public StatementAST
{
  DECLARE_AST_NODE(CatchStatement)

  ConditionAST* condition;
  StatementAST* statement;
};

struct TypeIdAST: public AST
{
  DECLARE_AST_NODE(TypeId)

  TypeSpecifierAST *type_specifier;
  DeclaratorAST *declarator;
};

///"typename"
struct TypeIdentificationAST: public ExpressionAST
{
  DECLARE_AST_NODE(TypeIdentification)

  std::size_t typename_token;
  NameAST *name;
  ExpressionAST *expression;
};

struct TypeParameterAST: public AST
{
  DECLARE_AST_NODE(TypeParameter)

  std::size_t type;
  NameAST *name;
  TypeIdAST *type_id;
  const ListNode<TemplateParameterAST*> *template_parameters;
  NameAST *template_name;
};

struct TypedefAST: public DeclarationAST
{
  DECLARE_AST_NODE(Typedef)

  TypeSpecifierAST *type_specifier;
  const ListNode<InitDeclaratorAST*> *init_declarators;
};

struct UnaryExpressionAST: public ExpressionAST
{
  DECLARE_AST_NODE(UnaryExpression)

  std::size_t op; //Index of the token that describes the operator
  ExpressionAST *expression;
};

struct UnqualifiedNameAST: public AST
{
  DECLARE_AST_NODE(UnqualifiedName)

  std::size_t tilde;
  std::size_t id;
  OperatorFunctionIdAST *operator_id;
  const ListNode<TemplateArgumentAST*> *template_arguments;
};

struct UsingAST: public DeclarationAST
{
  DECLARE_AST_NODE(Using)

  std::size_t type_name;
  NameAST *name;
};

struct UsingDirectiveAST: public DeclarationAST
{
  DECLARE_AST_NODE(UsingDirective)

  NameAST *name;
};

struct WhileStatementAST: public StatementAST
{
  DECLARE_AST_NODE(WhileStatement)

  ConditionAST *condition;
  StatementAST *statement;
};

struct WinDeclSpecAST: public AST
{
  DECLARE_AST_NODE(WinDeclSpec)

  std::size_t specifier;
  std::size_t modifier;
};

template <class _Tp>
_Tp *CreateNode(pool *memory_pool)
{
  _Tp *node = reinterpret_cast<_Tp*>(memory_pool->allocate(sizeof(_Tp)));
  node->kind = _Tp::__node_kind;
  return node;
}

template <class _Tp>
_Tp ast_cast(AST *item)
{
  if (item && static_cast<_Tp>(0)->__node_kind == item->kind)
    return static_cast<_Tp>(item);

  return 0;
}

#endif // AST_H

