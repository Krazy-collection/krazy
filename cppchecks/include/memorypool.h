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

#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include "rxx_allocator.h"
#include <cstring>

/**Memory pool for chars.*/
class pool
{
  rxx_allocator<char> __alloc;

public:
  /**Allocates the @p size bytes in the pool.*/
  inline void *allocate(std::size_t __size);
};

inline void *pool::allocate(std::size_t __size)
{
  return __alloc.allocate(__size);
}

#endif // MEMORYPOOL_H

