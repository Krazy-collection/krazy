/*
    Copyright (c) 2002-2003 KDE Team
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
/*
  This file is for the most of it a copy of kdemacros.h adjusted to be suitable
  for krazy.

  -- Bertjan, jan 2009
*/

#ifndef KRAZY_EXPORT_H
#define KRAZY_EXPORT_H

#ifdef __KRAZY_HAVE_GCC_VISIBILITY
#define KRAZY_NO_EXPORT __attribute__ ((visibility("hidden")))
#define KRAZY_EXPORT __attribute__ ((visibility("default")))
#define KRAZY_IMPORT __attribute__ ((visibility("default")))
#elif defined(_WIN32) || defined(_WIN64)
#define KRAZY_NO_EXPORT
#define KRAZY_EXPORT __declspec(dllexport)
#define KRAZY_IMPORT __declspec(dllimport)
#else
#define KRAZY_NO_EXPORT
#define KRAZY_EXPORT
#define KRAZY_IMPORT
#endif

#endif // KRAZY_EXPORT
