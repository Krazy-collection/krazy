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
#ifndef CHECK_ENGINE_H
#define CHECK_ENGINE_H

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QString>

#include <krazymacros.h>

#include "analyzer.h"

class QUrl;

/**
 * Small class which collects the results of the checks executed by the passed
 * visitor.
 */
class KRAZY_EXPORT CheckEngine
{
  public:
    /**
     * Creates a new CheckEngine object. This object takes ownership of the
     * visitor.
     */
    CheckEngine(Analyzer *analyzer);

    ~CheckEngine();

    /**
     * Processes the file and stores the result of the check.
     */
    void process(QUrl const &file);

    /**
     * Returns the results of the check.
     */
    QList<Result> results() const;

  protected: // Members
    QList<Result> m_results;

  private:
    Analyzer *m_analyzer;
};

#endif // CHECK_ENGINE_H
