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
#include <QtCore/QUrl>

#include <control.h>
#include <parser.h>
#include <parsesession.h>
#include <rpp/pp-engine.h>
#include <rpp/preprocessor.h>

CheckEngine::CheckEngine(CheckVisitor *vtr) : m_visitor(vtr)
{}

CheckEngine::~CheckEngine()
{
  delete m_visitor;
}

void CheckEngine::process(QUrl const &file)
{
  QByteArray contents = readAll(file);

  ParseSession session;
  rpp::Preprocessor preprocessor;
  rpp::pp pp(&preprocessor);
  PreprocessedContents ppContents;
  ppContents= pp.processFile(file.path(), contents);
  session.setContentsAndGenerateLocationTable(ppContents);

  Control control;
  Parser parzer(&control);
  TranslationUnitAST *ast = parzer.parse(&session);
  m_visitor->setParseSession(&session);
  m_visitor->visit(ast);
  m_results = m_visitor->analyze();
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

