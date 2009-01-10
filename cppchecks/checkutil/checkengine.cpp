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
#include "checkengine.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>

#include "Control.h"
#include "TranslationUnit.h"

CheckEngine::CheckEngine(CheckVisitor *vtr) : m_visitor(vtr)
{}

CheckEngine::~CheckEngine()
{
  delete m_visitor;
}

void CheckEngine::process(QUrl const &file)
{
  QByteArray contents = readAll(file);
  QFileInfo fileInfo(file.path());
  Control control;
  StringLiteral *fileId = control.findOrInsertFileName(fileInfo.fileName().toLatin1());
  TranslationUnit *unit = new TranslationUnit(&control, fileId);
  unit->setSource(contents, contents.length());
  unit->parse(TranslationUnit::ParseTranlationUnit);
}

QList<Result> CheckEngine::results() const
{
  return m_results;
}

QByteArray CheckEngine::readAll(QUrl const &filename)
{
  QFile f(filename.path());
  f.open(QIODevice::ReadOnly);
  return f.readAll();
}

