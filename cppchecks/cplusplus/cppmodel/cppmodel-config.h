/*
   Copyright (c) 2009 Bertjan Broeksema <b.broeksema@kdemail.net>

   GNU Lesser General Public License Usage

   This file may be used under the terms of the GNU Lesser
   General Public License version 2.1 as published by the Free Software
   Foundation and appearing in the file LICENSE.LGPL included in the
   packaging of this file.  Please review the following information to
   ensure the GNU Lesser General Public License version 2.1 requirements
   will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
*/

#ifndef CPLUSPLUS_CONFIG_H
#define CPLUSPLUS_CONFIG_H

#ifdef HAVE_QT
#  include <QtCore/qglobal.h>
#  define CPLUSPLUS_BEGIN_HEADER
#  define CPLUSPLUS_END_HEADER
#  if defined(CPLUSPLUSMODEL_BUILD_LIB)
#    define CPLUSPLUS_MODEL_EXPORT Q_DECL_EXPORT
#  else
#    define CPLUSPLUS_MODEL_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define CPLUSPLUS_BEGIN_HEADER
#  define CPLUSPLUS_END_HEADER
#  define CPLUSPLUS_EXPORT
#endif

#endif // CPLUSPLUS_CONFIG_H
