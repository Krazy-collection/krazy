/*
    Copyright (C) 2008 Bertjan Broeksema <b.broeksema@kdemail.net>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef DPOINTER_CHECK_AST_VISITOR
#define DPOINTER_CHECK_AST_VISITOR

#include <QtCore/QRegExp>

#include <default_visitor.h>
#include <parsesession.h>
#include <lexer.h>

#include "class.h"
#include "typedef.h"

class DPointerCheckASTVisitor : public CheckVisitor
{
  public:
    ////////////////////////// data members //////////////////////////////////
    QStack<Class>    m_classStack; // Used during visit
    QList<Class>     m_classes;    // Visited classes are stored here.

    bool             m_classNameRegistered;
    QString          m_className;
    bool             m_inTemplateDeclaration;
    Class::Scope     m_mode;
    TypeDef::DefType m_defType;
    QStringList      m_globalForwardDecls;
    QStringList      m_tempArgs;
    QString          m_varType;
    QString          m_varName;
    int              m_varLine;

    ////////////////////////// Constructor / Destructor //////////////////////

    DPointerCheckASTVisitor()
    : m_classNameRegistered(false), m_inTemplateDeclaration(false)
    {}

    virtual ~DPointerCheckASTVisitor()
    {}

    ////////////////////////// helpers ///////////////////////////////////////

    QString symbolForTokenId(std::size_t id)
    {
        return m_session->token_stream->token(id).symbolString();
    }

    int sourceLineForTokenId(std::size_t id)
    {
        std::size_t tokenPosition = m_session->token_stream->token(id).position;
        const rpp::Anchor& a = m_session->positionAt(tokenPosition);
        return a.line + 1; // +1 since editor-lines start from 1 while FooIndex starts from 0
    }

    void concatenateTokensBetween(int startToken, int endToken, QString& result)
    {
        if (startToken == 0) return;
        for (int i=startToken; i<=endToken; i++)
        {
            result += symbolForTokenId(i) + ' ';
        }

        result.chop(1);
    }

    void appendQualifiedNames(const ListNode<UnqualifiedNameAST*>* qualifiedNames, QString& name)
    {
        if (!qualifiedNames) return;

        const ListNode<UnqualifiedNameAST*> *it = qualifiedNames->toFront(), *end = it;
        do {
            name += "::";
            name += symbolForTokenId(it->element->id);
            it = it->next;
        } while (it!=end);
    }

    ////////////////////////// CheckVisitor method///////////////////////////

    virtual QList<Result> analyze() const
    {
      QList<Result> results;

      foreach (Class const &vclass, m_classes)
      {
//         QStringList fwdDecls = vclass.forwardDeclarations(Class::PRIVATE);
//         fwdDecls += vclass.forwardDeclarations(Class::PROTECTED);
//         QString privateName = QString(vclass.name() + "Private").remove(QRegExp(".*::"));

        // Finding a forward declaration which has the name ClassNamePrivate in
        // the protected or private section is a strong evidence that there is
        // a dpointer declared. We don't look in the public section though,
        // because a private class shouldn't be public.

        // First check the protected members.
        QList<TypeDef> typeDefs = vclass.typeDefenitions(Class::PROTECTED);
        bool hasDPointer = false;

        foreach (TypeDef const &typeDef, typeDefs)
        {
          if (isDPointer(typeDef, vclass))
          {
            hasDPointer = true;
            if (typeDef.defenitionType() != TypeDef::CONST_POINTER_TO_VALUE)
            {
              Result res;
              res.line = typeDef.line();
              res.longMessage = "non-const dpointer";
              results.append(res);
            }
          }
        }

        // Then check the private members.
        typeDefs = vclass.typeDefenitions(Class::PRIVATE);
        int privateMemberCount = 0;
        foreach (TypeDef const &typeDef, typeDefs)
        {
          if (isDPointer(typeDef, vclass))
          {
            hasDPointer = true;
            if (typeDef.defenitionType() == TypeDef::POINTER_TO_VALUE)
            {
              Result res;
              res.line = typeDef.line();
              res.longMessage = "non-const dpointer";
              results.append(res);
            }
          }
          else
          {
            ++privateMemberCount;
            Result res;
            res.line = typeDef.line();
            res.longMessage = "private members";
            results.append(res);
          }
        }

        if (!hasDPointer && privateMemberCount > 0)
        {
          Result res;
          res.line = -1; // We don't care.
          res.longMessage = "missing dpointer in classes";
          res.shortMessage = vclass.name();
          results.append(res);
        }
      }

      return results;
    }

    bool isDPointer(TypeDef const&typeDef, Class const &pclass) const
    {
      bool isConstPointer = typeDef.defenitionType() == TypeDef::CONST_POINTER_TO_VALUE;
      bool isPointer = typeDef.defenitionType() == TypeDef::POINTER_TO_VALUE;
      bool hasGlobalForwardDecl = m_globalForwardDecls.contains(typeDef.type());
      bool hasForwardDeclInClass = pclass.containsForwardDeclaration(typeDef.type());
      bool hasTemplateArgForwardDecl = false;
      bool isNamedD = typeDef.name() == "d";
      bool containsPrivate = typeDef.type().contains("Private");

      if (typeDef.templateArguments().size() == 1)
      {
        hasTemplateArgForwardDecl = 
          pclass.forwardDeclarations().contains(typeDef.templateArguments().first())
          || m_globalForwardDecls.contains(typeDef.templateArguments().first());
        containsPrivate = typeDef.templateArguments().first().contains("Private");
      }

      int score = 0;
      if (isConstPointer || isPointer) ++score;
      if (hasGlobalForwardDecl || hasForwardDeclInClass) ++score;
      if (hasTemplateArgForwardDecl) ++score;
      if (isNamedD) ++score;
      if (containsPrivate) ++score;

      return score >= 3;
    }

    ////////////////////////// visit overrides //////////////////////////////

    void visitClassSpecifier(ClassSpecifierAST *node)
    {
      // We're not interested in structs.
      if (symbolForTokenId(node->start_token) == "struct")
        return;

      m_classNameRegistered = false;
      m_mode = Class::PRIVATE; // By default members of a class are private.

      visit(node->win_decl_specifiers);
      visit(node->name);
//       visit(node->base_clause); // Don't visit base class defenitions.
      visitNodes(this, node->member_specs);

      m_classes.append(m_classStack.pop());
      m_classNameRegistered = m_classStack.isEmpty() ? false : true;
      m_className = m_classStack.isEmpty() ? QString() : m_classStack.top().name();
    }

    void visitAccessSpecifier(AccessSpecifierAST *node)
    {
      QString accessMode = symbolForTokenId(node->start_token);
      if (accessMode == "public")
        m_mode = Class::PUBLIC;
      else if (accessMode == "protected")
        m_mode = Class::PROTECTED;
      else
        m_mode = Class::PRIVATE;
    }

    void visitSimpleDeclaration(SimpleDeclarationAST *node)
    {
      QString token = symbolForTokenId(node->start_token);

      if (token == "class")
      {
        // Class definition of forward declaration.
        m_globalForwardDecls.append(symbolForTokenId(node->start_token + 1));
        DefaultVisitor::visitSimpleDeclaration(node);
      }
      else
      {
        // Variable defenition
        m_defType = TypeDef::VALUE;
        DefaultVisitor::visitSimpleDeclaration(node);
      }
    }

    void visitSimpleTypeSpecifier(SimpleTypeSpecifierAST *node)
    {
      m_varType = symbolForTokenId(node->start_token);
      m_tempArgs.clear();

      if (symbolForTokenId(node->start_token + 1) == "const"
        || symbolForTokenId(node->start_token - 1) == "const")
        m_defType = TypeDef::CONST_VALUE;

      DefaultVisitor::visitSimpleTypeSpecifier(node);
    }

    void visitPtrOperator(PtrOperatorAST* node)
    {
      if (symbolForTokenId(node->start_token) == "*") // Pointer
        if (symbolForTokenId(node->start_token + 1) == "const")
          if (m_defType == TypeDef::CONST_VALUE)
            m_defType = TypeDef::CONST_POINTER_TO_CONST_VALUE;
          else
            m_defType = TypeDef::CONST_POINTER_TO_VALUE;
        else
          if (m_defType == TypeDef::CONST_VALUE)
            m_defType = TypeDef::POINTER_TO_CONST_VALUE;
          else
            m_defType = TypeDef::POINTER_TO_VALUE;
      else // Reference
        if (symbolForTokenId(node->start_token + 1) == "const")
          if (m_defType == TypeDef::CONST_VALUE)
            m_defType = TypeDef::CONST_REFERENCE_TO_CONST_VALUE;
          else
            m_defType = TypeDef::CONST_REFERENCE_TO_VALUE;
        else
          if (m_defType == TypeDef::CONST_VALUE)
            m_defType = TypeDef::REFERENCE_TO_CONST_VALUE;
          else
            m_defType = TypeDef::REFERENCE_TO_VALUE;

      DefaultVisitor::visitPtrOperator(node);
    }

    void visitName(NameAST *node)
    {
      if (symbolForTokenId(node->start_token - 1) == "explicit")
        return; // This is a constructor.

      if (symbolForTokenId(node->end_token) == "(")
        return; // Function call

      if ((symbolForTokenId(node->start_token - 1) == "class" 
           || symbolForTokenId(node->start_token - 1) == "struct")
          && symbolForTokenId(node->end_token) == ";")
        if (m_classStack.isEmpty())
         return;
        else
        {
          QString name;
          appendQualifiedNames(node->qualified_names, name);

          if (!name.isEmpty())
            name += "::" + symbolForTokenId(node->unqualified_name->id);
          else
            name = symbolForTokenId(node->unqualified_name->id);
          m_classStack.top().addForwardDeclaration(m_mode, name);
        }
      else if (!m_classNameRegistered && !m_inTemplateDeclaration)
      {
        m_className = symbolForTokenId(node->start_token);
        if (!m_classStack.isEmpty())
          m_className.prepend(m_classStack.top().name() + "::");

        m_classNameRegistered = true;
        m_classStack.push(Class(m_className));
      }
      else if(symbolForTokenId(node->start_token) != m_varType && !m_inTemplateDeclaration)
      {
        m_varName = symbolForTokenId(node->start_token);
        m_varLine = sourceLineForTokenId(node->start_token);
        TypeDef typeDef(m_varType, m_defType, m_varName);
        typeDef.setLine(sourceLineForTokenId(node->start_token));
        typeDef.setTemplateArguments(m_tempArgs);
        m_tempArgs.clear();

        m_classStack.top().addTypeDef(m_mode, typeDef);
      }

      // Make sure that we visit eventual template arguments.
      DefaultVisitor::visitName(node);
    }

    void visitTypedef(TypedefAST *)
    {
      // Don't visit typedefs.
    }

    void visitParameterDeclaration(ParameterDeclarationAST *)
    {
      // Don't visit parameter declarations.
    }

    void visitCompoundStatement(CompoundStatementAST *)
    {
      // Don't visit compount statements in header files. This prevents that
      // lines like:
      //
      // const Private * impl() const { return d; }
      // 
      // get reported.
    }

    void visitTemplateArgument(TemplateArgumentAST *node)
    {
      // Store template arguments.
      m_tempArgs.append(symbolForTokenId(node->start_token));
    }

    void visitTemplateParameter(TemplateParameterAST *)
    {
      // Don't visit template parameters.
    }

    void visitTemplateDeclaration(TemplateDeclarationAST *node)
    {
      m_inTemplateDeclaration = true;
      DefaultVisitor::visitTemplateDeclaration(node);
      m_inTemplateDeclaration = false;
    }

    void visitUsing(UsingAST *)
    {
      // Don't visit using.
    }
    
    void visitEnumSpecifier(EnumSpecifierAST *)
    {
      // Don't visit enum specifiers.
    }
};

#endif
