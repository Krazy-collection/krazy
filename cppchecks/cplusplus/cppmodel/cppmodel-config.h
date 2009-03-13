// Copyright (c) 2009 Bertjan Broeksema <b.broeksema@home.nl>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

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
