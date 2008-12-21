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
#include "check.h"

#include <QtCore/QFile>
#include <QtCore/QUrl>

#include <iostream>

#include "checkengine.h"
#include "outputformatter.h"

using namespace std;

Check::Check(int argc, char **argv) 
: m_action(RUN_CHECK),
  m_installed(false),
  m_isValid(false),
  m_mode(NORMAL)
{
  m_isValid = parseArguments(argc, argv);
}

/* virtual */ Check::~Check()
{}

bool Check::argumentsValid() const
{
  return m_isValid;
}

void Check::run()
{
  switch (m_action)
  {
    case RUN_CHECK:
      runCheck();
      break;
    case PRINT_VERSION:
      printVersion();
      break;
    case PRINT_EXPLANATION:
      printExplanation();
      break;
    default:
      printHelp();
      break;
  }
}

/* virtual */ void Check::printHelp() const
{
  cout << "usage:" << qPrintable(m_checkName) << " <options> <source-file>" << endl;
  cout << "Where options is contains one or more of the following items:" << endl;
  cout << "  Output mode (Default is normal):" << endl;
  cout << "    --verbose   Verbose output mode" << endl;
  cout << "    --quiet     Quiet output mode" << endl;
  cout << "  Program action (Default is run check):" << endl;
  cout << "    --explain   Print the reason behind this check" << endl;
  cout << "    --help      Print this help" << endl;
  cout << "  Other:" << endl;
  cout << "    --installed This is to let the test know whether the source-file " << endl;
  cout << "                is installed by the buildsystem." << endl;
}

bool Check::parseArguments(int argc, char **argv)
{
  m_checkName = QString(argv[0]);

  if (argc == 1)
  {
    // We require at least a file name.
    m_action = PRINT_HELP;
    return false;
  }

  for( --argc, ++argv; argc; --argc, ++argv )
  {
    // Output mode
    if (!strcmp(*argv, "--verbose"))
      m_mode = VERBOSE;
    else if (!strcmp( *argv, "--quiet"))
      m_mode = QUIET;
    // Action
    else if (!strcmp( *argv, "--version"))
    {
      m_action = PRINT_VERSION;
      return true;
    }
    else if (!strcmp( *argv, "--help"))
    {
      m_action = PRINT_HELP;
      return true;
    }
    else if( !strcmp( *argv, "--explain" ))
    {
      m_action = PRINT_EXPLANATION;
      return true;
    }
    // Other
    else if (!strcmp( *argv, "--installed"))
      m_installed = true;
    // And last, the file name.
    else {
      m_fileName = QString::fromLatin1( *argv );
      break;
    }
  }

  QFile f(m_fileName);
  return f.exists();
}

void Check::runCheck()
{
  CheckEngine *engine = createCheckEngine();
  engine->process(QUrl(m_fileName));

  QList<Result> results = engine->results();

  if (results.empty() && m_mode != QUIET)
    cout << "okay!" << endl;
  else
  {
    OutputFormatter *formatter = createOutputFormatter();
    switch (m_mode)
    {
      case VERBOSE:
        // print issues verbose.
        formatter->printResultsVerbose(results);
        break;
      default:
        // print issues normal.
        formatter->printResults(results);
        break;
    }

    delete formatter;
    formatter = 0;
  }

  delete engine;
  engine = 0;
}
