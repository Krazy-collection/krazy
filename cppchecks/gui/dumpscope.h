#ifndef DUMP_SCOPE_H
#define DUMP_SCOPE_H

#ifdef Q_CC_GNU
#include <cxxabi.h>
#endif

#include <QtCore/QIODevice>

#include "NamePrettyPrinter.h"
#include "Overview.h"
#include "Scope.h"
#include "Symbol.h"
#include "SymbolVisitor.h"

class DumpScope : protected SymbolVisitor
{
  int               m_depth;
  QIODevice        *m_device;
  NamePrettyPrinter m_namePrinter;
  
  public:
    DumpScope(QIODevice *device)
      : m_depth(0)
      , m_device(device)
      , m_namePrinter(new Overview())
    {}

    void operator()(Scope const &scope)
    {
      m_device->write("Namespace (Anonymous)\n");
      m_depth++;

      for (unsigned idx = 0; idx < scope.symbolCount(); ++idx)
      {
        accept(scope.symbolAt(idx));
      }

      m_depth--;
    }

  protected:
    virtual bool preVisit(Symbol *symbol)
    {
      const char *name;
      #ifdef Q_CC_GNU
      name = abi::__cxa_demangle(typeid(*symbol).name(), 0, 0, 0) + 11;
      #else
      name = typeid(*symbol).name();
      #endif

      QString data(m_depth, ' ');
      data += QString(name);
      data += " (" + m_namePrinter(symbol->name());
      data += ")\n";
      m_device->write(data.toUtf8());
      
      ++m_depth;
      return true;
    }

    virtual void postVisit(Symbol *)
    { --m_depth; }
};

#endif // DUMP_AST_H
