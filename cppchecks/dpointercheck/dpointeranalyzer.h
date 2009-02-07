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
#ifndef DPOINTER_ANALYZER_H
#define DPOINTER_ANALYZER_H

#include "analyzer.h"

namespace CPlusPlus {
  class Class;
  class Scope;
}

class DPointerAnalyzer : public Analyzer
{
  public:
    ~DPointerAnalyzer() {}

    /* virtual */ QList<Result> analyze(CPlusPlus::Scope const &globalScope);

  private: // Methods
    void analyzeClass(CPlusPlus::Class const * const cls);

    void analyzeScope(CPlusPlus::Scope const * const ns);

  private: // Members
    QList<Result> m_issues;
};

#endif // DPOINTER_ANALYZER_H
