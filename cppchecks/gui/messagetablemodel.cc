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

#include "messagetablemodel.h"

using namespace CPlusPlus;
using namespace CppModel;

MessageTableModel::MessageTableModel(QList<DiagnosticMessage> const &messages)
  : m_messages(messages)
{}

int MessageTableModel::columnCount(QModelIndex const &parent) const
{
  return 2;
}

QVariant MessageTableModel::data(QModelIndex const &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role == Qt::DisplayRole)
  {
    switch (index.column())
    {
      case 0:
        return m_messages.at(index.row()).line();
      default:
        return m_messages.at(index.row()).text();
    }
  }

  return QVariant();
}

Qt::ItemFlags MessageTableModel::flags(QModelIndex const &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant MessageTableModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const
{
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    return QVariant();

  switch (section)
  {
    case 0:
      return QString("Line");
    case 1:
      return QString("Message");
    default:
      return QVariant();
  }
}

int MessageTableModel::rowCount(QModelIndex const &parent) const
{
  return m_messages.size();
}
