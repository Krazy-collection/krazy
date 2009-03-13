// Copyright (c) 2009 Bertjan Broeksema <b.broeksema@home.nl>
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
#ifndef CPLUSPLUS_CPPMODEL_CHARBLOCK_H
#define CPLUSPLUS_CPPMODEL_CHARBLOCK_H

namespace CPlusPlus {

namespace CppModel {

  /**
   * Block represents a block of characters in a Document.
   */
  class CharBlock
  {
    public:
      CharBlock(unsigned begin = 0, unsigned end = 0);

      unsigned begin() const;

      bool contains(unsigned pos) const;

      unsigned end() const;

    private:
      unsigned m_begin;
      unsigned m_end;
  };

  inline CharBlock::CharBlock(unsigned begin, unsigned end)
    : m_begin(begin), m_end(end)
  {}

  inline unsigned CharBlock::begin() const
  {
    return m_begin;
  }

  inline bool CharBlock::contains(unsigned pos) const
  {
    return pos >= m_begin && pos < m_end;
  }

  inline unsigned CharBlock::end() const
  {
    return m_end;
  }

} // namespace CppModel

} // namespace CPlusPlus

#endif // CPLUSPLUS_CPPMODEL_CHARBLOCK_H
