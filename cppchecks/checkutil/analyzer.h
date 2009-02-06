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
#ifndef ANALYZER_H
#define ANALYZER_H

#include <krazymacros.h>

class Control;
class Scope;

struct Result
{
  int line;
  QString longMessage;
  QString shortMessage; // Will be used to form lines like: file.cpp lines#123[shortMessage],125[issue]
};

class KRAZY_EXPORT Analyzer
{
  public:
    virtual ~Analyzer() 
    {}

    /**
     * Analyzes the semantic information gathered during parsing and generates a
     * list with the final results of the check.
     */
    virtual QList<Result> analyze(Scope const &globalScope) const = 0;
};

#endif // ANALYZER_H
