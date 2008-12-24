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

#include "dpointercheckoutputformatter.h"

#include <iostream>

#include <QtCore/QMap>
#include <QtCore/QSet>

using namespace std;

/*virtual*/ void DPointerCheckOutputFormatter::printResults(QList<Result> const &results) const
{
  QMap<QString, QSet<int> > lineIssues;
  QMap<QString, QList<QString> > generalIssues;

  foreach (Result const &result, results)
  {
    if (result.line == -1)
      generalIssues[result.longMessage].append(result.shortMessage);
    else
      lineIssues[result.longMessage].insert(result.line);
  }

  foreach (QString const &issue, lineIssues.keys())
  {
    QList<int> lines = QList<int>::fromSet(lineIssues[issue]);
    qSort(lines.begin(), lines.end());
    QString linesStr;

    foreach (int line, lines)
      linesStr.append(',' + QString::number(line));

    linesStr.remove(0,1); // Remove the first comma
    cout << qPrintable(issue) << " line#" << qPrintable(linesStr) << " (";
    cout << lines.size() << ')' << endl;
  }

  foreach (QString const &issue, generalIssues.keys())
  {
    QList<QString> messages = generalIssues[issue];
    QString messageStr;

    foreach (QString const &message, messages)
      messageStr.append(',' + message);

    messageStr.remove(0,1); // Remove the first comma
    cout << qPrintable(issue) << ": " << qPrintable(messageStr) << " (";
    cout << messages.size() << ')' <<  endl;
  }
}

/*virtual*/ void DPointerCheckOutputFormatter::printResultsVerbose(QList<Result> const &results) const
{
  Q_UNUSED(results);
//   foreach (Result const &result, results)
//   {
//     
//   }
}
