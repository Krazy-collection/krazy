/* This file is part of KDevelop
    Copyright 2002-2005 Roberto Raggi <roberto@kdevelop.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SYMBOL_H
#define SYMBOL_H

#include <QtCore/QString>
#include <cstring>

#include <QtCore/QHash>
#include <QtCore/QPair>

/**@file symbol.h Symbol table for the parser.*/

/**
Symbol in the parser symbol table.
*/
struct NameSymbol
{
  const char *data;
  std::size_t count;

  inline QString as_string() const
  {
    return QString::fromUtf8(data, count);
  }

  /**Required to put NameSymbol's into a QHash.*/
  inline bool operator == (const NameSymbol &other) const
  {
    return count == other.count
      && std::strncmp(data, other.data, count) == 0;
  }

protected:
  inline NameSymbol() {}
  inline NameSymbol(const char *d, std::size_t c)
    : data(d), count(c) {}

private:
  void operator = (const NameSymbol &);

  friend class NameTable;
};


/**
@brief Required to put NameSymbol's into a QHash.
@see qHash(const QPair<const char*, std::size_t> &r) method for the reference.*/
inline uint qHash(const NameSymbol &r)
{
  uint hash_value = 0;

  for (std::size_t i=0; i<r.count; ++i)
    hash_value = (hash_value << 5) - hash_value + r.data[i];

  return hash_value;
}


/**
Required to put pairs of char* and std::size_t into a QHash.

Hash function is: hash = hash*31 + key[i].@n
It looks like a Bernstein's hash function with a different factor.
@n Original Bernstein's function is: hash = hash*33 + key[i].
@n Factor of 31 makes this function return reliably unique values
for english words with 6 symbols length.

@n More information can be found at @a http://burtleburtle.net/bob/hash/doobs.html.
*/
inline uint qHash(const QPair<const char*, std::size_t> &r)
{
  uint hash_value = 0;

  for (std::size_t i=0; i<r.second; ++i)
    hash_value = (hash_value << 5) - hash_value + r.first[i];

  return hash_value;
}

/**Symbol table for the parser.*/
class NameTable
{
public:
  typedef QPair<const char *, std::size_t> KeyType;
  typedef QHash<KeyType, NameSymbol*> ContainerType;

public:
  NameTable() {}

  ~NameTable()
  {
    qDeleteAll(_M_storage);
  }

  inline const NameSymbol *findOrInsert(const char *str, std::size_t len)
  {
    KeyType key(str, len);

    NameSymbol *name = _M_storage.value(key);
    if (!name)
      {
        name = new NameSymbol(str, len);
        _M_storage.insert(key, name);
      }

    return name;
  }

  inline std::size_t count() const
  {
    return _M_storage.size();
  }

private:
  ContainerType _M_storage;

private:
  NameTable(const NameTable &other);
  void operator = (const NameTable &other);
};

#endif // SYMBOL_H

