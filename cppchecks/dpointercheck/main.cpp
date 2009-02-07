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
#include <iostream>

#include "dpointercheck.h"

using std::cerr;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
  /*
    * Assumptions:
    * - The file parsed is a header file.
    * - The file is (going to be) installed.
    * - All private member variables that are not pointers or that are pointers
    *   but don't have a name like FooPrivate are considered as variables that
    *   should be in the d-pointer.
    */
  DPointerCheck check(argc, argv);
  return check.run();
}
