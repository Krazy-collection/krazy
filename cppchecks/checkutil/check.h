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
#ifndef CHECK_H
#define CHECK_H

#include <QtCore/QString>

#include <krazymacros.h>

class CheckEngine;
class OutputFormatter;

class KRAZY_EXPORT Check
{
  enum Action
  {
    PRINT_VERSION,
    PRINT_HELP,
    PRINT_EXPLANATION,
    RUN_CHECK,
  };

  enum Mode
  {
    QUIET,
    NORMAL,
    VERBOSE
  };

  enum PriorityType
  {
    PT_ALL,       // low+normal+high
    PT_LOW,       // low only
    PT_NORMAL,    // normal only
    PT_IMPORTANT, // low+normal
    PT_HIGH       // high only
  };

  enum StrictType
  {
    ST_ALL,
    ST_SUPER,
    ST_NORMAL
  };

  public:
    Check(int argc, char **argv);

    virtual ~Check();

    /**
     * Returns true when the arguments past to the constructor are valid.
     */
    bool argumentsValid() const;

    /**
     * Run the check. Returns the total number of issues found.
     */
    int run();

  protected:
    /**
     * Creates a new check engine. Caller must take ownership of the pointer.
     */
    virtual CheckEngine * createCheckEngine() = 0;

    /**
     * Creates a new OutputFormatter. Caller must take ownership of the pointer.
     */
    virtual OutputFormatter * createOutputFormatter() = 0;

    virtual void printExplanation() const = 0;

    virtual void printHelp() const = 0;

    void printUsage() const;

    virtual void printVersion() const = 0;

    /**
     * Indicates wheter or not the check is only mentioned for files that are
     * installed.
     */
    virtual bool processInstalledFilesOnly() const = 0;

  private: // Functions
    bool parseArguments(int argc, char **argv);

    void runCheck();

    void setPriorityType(QString const &priority);

    void setStrictType(QString const &priority);

    bool validatePriorityType(QString const &priority) const;

    bool validateStrictType(QString const &strict) const;

  private: // Members
    Action       m_action;
    QString      m_checkName;
    QString      m_fileName;
    bool         m_installed;
    uint         m_issueCount;
    bool         m_isValid;
    Mode         m_mode;
    PriorityType m_priority;
    StrictType   m_strictness;
};

#endif // CHECK_H
