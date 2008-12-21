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
#ifndef CHECK_VISITOR_H
#define CHECK_VISITOR_H

#include <kdemacros.h>

#include <default_visitor.h>

class ParseSession;

struct Result
{
  int line;
  QString longMessage;
  QString shortMessage; // Will be used to form lines like: file.cpp lines#123[shortMessage],125[issue]
};

class KDE_EXPORT CheckVisitor : public DefaultVisitor
{
  public:
    CheckVisitor() : m_session(0)
    {}

    virtual ~CheckVisitor() 
    {}

    /**
     * Analyzes the information gathered during the visit and generates a list
     * with the final results of the check.
     */
    virtual QList<Result> analyze() const = 0;

    /**
     * Sets the parse session which will be used while visiting the AST. 
     * This class will not take ownership of the pointer.
     */
    void setParseSession(ParseSession *session)
    {
      m_session = session;
    }

  protected:
    ParseSession *m_session;
};

#endif
