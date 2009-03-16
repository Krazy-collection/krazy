/*
   This file or code within this file was originally part of Qt Creator.

   Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
   Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
   Copyright (c) 2009 Bertjan Broeksema <b.broeksema@kdemail.net>

   GNU Lesser General Public License Usage

   This file may be used under the terms of the GNU Lesser
   General Public License version 2.1 as published by the Free Software
   Foundation and appearing in the file LICENSE.LGPL included in the
   packaging of this file.  Please review the following information to
   ensure the GNU Lesser General Public License version 2.1 requirements
   will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
*/
#ifndef CPLUSPLUS_CPPMODEL_OVERVIEW_H
#define CPLUSPLUS_CPPMODEL_OVERVIEW_H

#include <parser/CPlusPlusForwardDeclarations.h>

#include <QtCore/QString>

#include "cppmodel-config.h"

namespace CPlusPlus {

namespace CppModel {

  class CPLUSPLUS_MODEL_EXPORT Overview
  {
    public:
      Overview();

      ~Overview();

      QString prettyName(Name *name) const;

      QString prettyType(FullySpecifiedType const &type, Name *name = 0) const;

      QString prettyType(FullySpecifiedType const &type, QString const &name) const;

      void setMarkArgument(unsigned position); // 1-based

      void setShowArgumentNames(bool showArgumentNames);

      void setShowFunctionSignatures(bool showFunctionSignatures);

      void setShowReturnTypes(bool showReturnTypes);

      bool showArgumentNames() const;

      bool showFunctionSignatures() const;

      bool showReturnTypes() const;

      QString operator()(Name *name) const;

      QString operator()(FullySpecifiedType const &type, Name *name = 0) const;

    private: // Functions
      Overview(Overview const &other);

      void operator =(Overview const &other);

    private: // Members
      unsigned m_markArgument;
      bool     m_showArgumentNames: 1;
      bool     m_showReturnTypes: 1;
      bool     m_showFunctionSignatures: 1;
  };

} // namespace CppModel

} // namespace CPlusPlus

#endif // CPLUSPLUS_CPPMODEL_OVERVIEW_H
