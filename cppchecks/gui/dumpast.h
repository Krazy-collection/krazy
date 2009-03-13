#ifndef DUMP_AST_H
#define DUMP_AST_H

#ifdef Q_CC_GNU
#include <cxxabi.h>
#endif

#include <QtCore/QIODevice>

#include <parser/AST.h>
#include <parser/ASTVisitor.h>
#include <parser/Control.h>
#include <parser/PrettyPrinter.h>

class DumpAST : protected ASTVisitor
{
  int m_depth;
  QIODevice *m_device;
  
  public:
    DumpAST(QIODevice *device, Control *control)
        : ASTVisitor(control), m_depth(0), m_device(device)
    {}

    void operator()(AST *ast)
    { accept(ast); }

  protected:
    virtual bool preVisit(AST *ast)
    {
      const char *name;
      #ifdef Q_CC_GNU
      name = abi::__cxa_demangle(typeid(*ast).name(), 0, 0, 0) + 11;
      #else
      name = typeid(*ast).name();
      #endif

      QString data(m_depth, ' ');
      data += QString(name) + '\n';
      m_device->write(data.toUtf8());
      
      ++m_depth;
      return true;
    }

    virtual void postVisit(AST *)
    { --m_depth; }
};

#endif // DUMP_AST_H
