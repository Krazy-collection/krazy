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
#include <QtCore/QStringList>
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
  if (!m_isValid)
  {
    printUsage();
    return;
  }

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

/* virtual */ void Check::printUsage() const
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
  cout << "    --strict=STRICTNESS" << endl;
  cout << "                This determines how strict the checker should be for finding " << endl;
  cout << "                issues. Supported for values are: normal, super, all" << endl;
  cout << "    --priority=PRIORITY" << endl;
  cout << "                This determines the issues that should be reported" << endl;
  cout << "                Supported values for PRIORITY are: low, normal, important, high, all" << endl;
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

  bool isKrazy = false;
  bool strictArgPassed = false;
  bool priorArgPassed = false;
  for( --argc, ++argv; argc; --argc, ++argv )
  {
    if (strictArgPassed)
    {
      if (validateStrictType(*argv))
      {
        setStrictType(*argv);
        strictArgPassed = false;
      }
      else
        return false;
    }
    else if (priorArgPassed)
    {
      if (validatePriorityType(*argv))
      {
        setPriorityType(*argv);
        priorArgPassed = false;
      }
      else
        return false;
    }
    else if (!strcmp(*argv,"--strict"))
    {
      strictArgPassed = true;
    }
    else if (!strcmp(*argv,"--priority"))
      priorArgPassed = true;
    else if(!strcmp(*argv, "--krazy"))
      isKrazy = true;
    // Output mode
    else if (!strcmp(*argv, "--verbose"))
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

  if ( !isKrazy )
  {
    cout << "Checker is not called as part of Krazy... exiting" << endl;
    return false;
  }

  QFile f(m_fileName);
  return f.exists();
}

void Check::runCheck()
{
  if (processInstalledFilesOnly() && !m_installed)
  {
    cerr << "SKIPPING FILE: " << qPrintable(m_fileName) << endl;
    return;
  }
  cerr << "CHECKING FILE: " << qPrintable(m_fileName) << endl;

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

void Check::setPriorityType(QString const &priority)
{
  // We assume a string in the form of --priority=all|low|normal|important|high
  // and thus that validateStrictType(priority) returns true.
  QString prior = priority.toLower();

  if (prior == "low")
    m_priority = PT_LOW;
  else if (prior == "normal")
    m_priority = PT_NORMAL;
  else if (prior == "important")
    m_priority = PT_IMPORTANT;
  else if (prior == "high")
    m_priority = PT_HIGH;
  else
    m_priority = PT_ALL;
}

void Check::setStrictType(QString const &strict)
{
  // We assume a string in the form of --strict=all|super|normal
  QString s = strict.toLower();

  if (s == "normal")
    m_strictness = ST_NORMAL;
  else if (s == "super")
    m_strictness = ST_SUPER;
  else
    m_strictness = ST_ALL;
}

bool Check::validatePriorityType(QString const &priority) const
{
  cerr << "Check::validatePriorityType() " << qPrintable(priority) << endl;
  if (priority.isEmpty())
    return false;

  QString p = priority.toLower();
  if ((p == "all") ||        // low+normal+high
      (p == "low") ||        // low only
      (p == "normal") ||     // normal only
      (p == "important") ||  // low+normal
      (p == "high"))         // high only
    return true;

  return false;
}

bool Check::validateStrictType(QString const &strict) const
{
  if (strict.isEmpty())
    return false;

  QString s = strict.toLower();
  if ((s == "all") ||        // super+normal
      (s == "super") ||      // super only
      (s == "normal"))       // normal only
    return true;

  return false;
}
