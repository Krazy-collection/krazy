/*
    Copyright (C) 2009 Bertjan Broeksema <b.broeksema@kdemail.net>

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

class DumpAST : protected CPlusPlus::ASTVisitor
{
  int m_depth;
  QIODevice *m_device;
  
  public:
    DumpAST(QIODevice *device, CPlusPlus::Control *control)
        : CPlusPlus::ASTVisitor(control), m_depth(0), m_device(device)
    {}

    void operator()(CPlusPlus::AST *ast)
    { accept(ast); }

  protected:
    virtual bool preVisit(CPlusPlus::AST *ast)
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

    virtual void postVisit(CPlusPlus::AST *)
    { --m_depth; }
};

#endif // DUMP_AST_H
