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

#include "dpointercheck.h"

#include <QtCore/QFile>

#include <iostream>

#include "checkengine.h"
#include "dpointercheckastvisitor.h"
#include "dpointercheckoutputformatter.h"

using namespace std;

#define VERSION "0.1"

DPointerCheck::DPointerCheck(int argc, char **argv)
: Check(argc,argv),
  m_version(VERSION)
{}

DPointerCheck::~DPointerCheck()
{}

/* virtual */ CheckEngine * DPointerCheck::createCheckEngine()
{
  return new CheckEngine(new DPointerCheckASTVisitor());
}

/*virtual*/ OutputFormatter * DPointerCheck::createOutputFormatter()
{
  return new DPointerCheckOutputFormatter();
}

/* virtual */ void DPointerCheck::printHelp() const
{
  cout << "Check public classes with private members or d-pointer issues";
}

/* virtual */ void DPointerCheck::printExplanation() const
{
  cout << "In order to more easily maintain binary compatibility, a public class "
       << "in an installed header should not contain private members -- use"
       << "d-pointers instead. Application headers should not mix d-pointers and "
       << "private members. Also ensure  that the d-pointer is \'const\' to "
       << "avoid modifying it by mistake. Please follow the guidelines in the "
       << "d-pointers section of "
       << "<http://techbase.kde.org/Policies/Library_Code_Policy#D-Pointers>.\n";
}

/* virtual */ void DPointerCheck::printVersion() const
{
  cout << "DPointerCheck version " << m_version.data() << endl;
}
