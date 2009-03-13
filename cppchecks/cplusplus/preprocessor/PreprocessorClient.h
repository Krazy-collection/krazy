/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
**************************************************************************/

#ifndef CPLUSPLUS_PP_CLIENT_H
#define CPLUSPLUS_PP_CLIENT_H

#include <parser/CPlusPlusForwardDeclarations.h>
#include <QtCore/QVector>

QT_BEGIN_NAMESPACE
class QByteArray;
class QString;
QT_END_NAMESPACE

namespace CPlusPlus {

class Macro;

class CPP_PREPROCESSOR_EXPORT MacroArgumentReference
{
  unsigned _position;
  unsigned _length;

public:
  MacroArgumentReference(unsigned position = 0, unsigned length = 0)
    : _position(position), _length(length)
  { }

  unsigned position() const
  { return _position; }

  unsigned length() const
  { return _length; }
};

class CPP_PREPROCESSOR_EXPORT Client
{
  Client(const Client &other);
  void operator=(const Client &other);

public:
  enum IncludeType {
    IncludeLocal,
    IncludeGlobal
  };

public:
  Client();
  virtual ~Client();

  virtual void macroAdded(const Macro &macro) = 0;
  virtual void sourceNeeded(QString &fileName, IncludeType mode,
                            unsigned line) = 0; // ### FIX the signature.

  virtual void startExpandingMacro(unsigned offset,
                                   const Macro &macro,
                                   const QByteArray &originalText,
                                   const QVector<MacroArgumentReference> &actuals
                                            = QVector<MacroArgumentReference>()) = 0;

  virtual void stopExpandingMacro(unsigned offset,
                                  const Macro &macro) = 0;

  virtual void startSkippingBlocks(unsigned offset) = 0;
  virtual void stopSkippingBlocks(unsigned offset) = 0;
};

} // namespace CPlusPlus

#endif // CPLUSPLUS_PP_CLIENT_H
