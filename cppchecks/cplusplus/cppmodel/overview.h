/*
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
