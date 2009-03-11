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
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
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

#ifndef CPLUSPLUS_ARRAY_H
#define CPLUSPLUS_ARRAY_H

#include "CPlusPlusForwardDeclarations.h"
#include <new>
#include <cstdlib>

CPLUSPLUS_BEGIN_HEADER
CPLUSPLUS_BEGIN_NAMESPACE

template <typename _Tp, int SEGMENT_SHIFT = 4>
class Array
{
    Array(const Array &other);
    void operator =(const Array &other);

public:
    Array()
        : _segments(0),
          _allocatedSegments(0),
          _segmentCount(-1),
          _allocatedElements(0),
          _count(-1)
    { }

    ~Array()
    {
        if (_segments) {
            for (int index = 0; index <= _segmentCount; ++index) {
                delete[] (_segments[index] + (index << SEGMENT_SHIFT));
            }
            free(_segments);
        }
    }

    inline unsigned size() const
    { return _count + 1; }

    inline unsigned count() const
    { return _count + 1; }

    inline const _Tp &at(unsigned index) const
    { return _segments[index >> SEGMENT_SHIFT][index]; }

    inline const _Tp &operator[](unsigned index) const
    { return _segments[index >> SEGMENT_SHIFT][index]; }

    inline _Tp &operator[](unsigned index)
    { return _segments[index >> SEGMENT_SHIFT][index]; }

    void push_back(const _Tp &value)
    {
        if (++_count == _allocatedElements) {
            if (++_segmentCount == _allocatedSegments) {
                _allocatedSegments += 4;
                _segments = (_Tp **) realloc(_segments, _allocatedSegments * sizeof(_Tp *));
            }

            _Tp *segment = new _Tp[SEGMENT_SIZE];
            _segments[_segmentCount] = segment - (_segmentCount << SEGMENT_SHIFT);
            _allocatedElements += SEGMENT_SIZE;
        }

        _segments[_count >> SEGMENT_SHIFT][_count] = value;
    }

private:
    enum {
        SEGMENT_SIZE = 1 << SEGMENT_SHIFT
    };

    _Tp **_segments;
    int _allocatedSegments;
    int _segmentCount;

    int _allocatedElements;
    int _count;
};

CPLUSPLUS_END_NAMESPACE
CPLUSPLUS_END_HEADER

#endif // CPLUSPLUS_ARRAY_H
