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
#ifndef PARSE_RESULT_WIDGET_H
#define PARSE_RESULT_WIDGET_H

#include <QtGui/QWidget>

#include <cppmodel/document.h>
#include <parser/Scope.h>

class IncludesTreeModel;
class QAbstractItemModel;
class QModelIndex;

namespace CPlusPlus
{
class Scope;
}

namespace Ui
{
  class ParseResultWidget;
}

class ParseResultWidget : public QWidget
{
  Q_OBJECT

  public:
    ParseResultWidget();
    ~ParseResultWidget();

  protected slots:
    void exportAST();
    void exportScope();
    void onASTItemClicked(QModelIndex const &);
    void onIncludeClicked(QModelIndex const &);
    void onStateChanged(int);
    void onTreeTypeChanged(int index);
    void openFile();
    void openIncConfig();

  private: // Functions
    QStringList globalIncludePaths() const;
    QStringList localIncludePaths() const;

  private:
    IncludesTreeModel                 *m_includeTreeModel;
    CPlusPlus::CppModel::Document::Ptr m_rootDoc;
    CPlusPlus::CppModel::Document::Ptr m_selectedDoc;
    Ui::ParseResultWidget             *m_ui;
    CPlusPlus::Scope                  *m_globals;
};

#endif // PARSE_RESULT_WIDGET_H
