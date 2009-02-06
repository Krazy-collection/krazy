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
#ifndef OUTPUT_FORMATTER_H
#define OUTPUT_FORMATTER_H

#include <QtCore/QList>

#include <krazymacros.h>

#include "analyzer.h"

class KRAZY_EXPORT OutputFormatter
{
  public:
    virtual ~OutputFormatter()
    {}

    virtual void printResults(QList<Result> const &results) const = 0;

    virtual void printResultsVerbose(QList<Result> const &results) const = 0;
};

#endif // OUTPUT_FORMATTER_H
