/*************************************************************************************
 *                                                                                   *
 *               __   __               _____             _                           *
 *               \ \ / /   _ _ __ ___ | ____|_ __   __ _(_)_ __   ___                *
 *                \ V / | | | '_ ` _ \|  _| | '_ \ / _` | | '_ \ / _ \               *
 *                 | || |_| | | | | | | |___| | | | (_| | | | | |  __/               *
 *                 |_| \__,_|_| |_| |_|_____|_| |_|\__, |_|_| |_|\___|               *
 *                                                 |___/                             *
 *                                                                                   *
 *                                   By YumStudio                                    *
 *                                  Lead by モノエ.                                   * 
 *                                                                                   *
 *                                All rights reserved                                *
 *                            This file is free & open source,                       *
 *                             And covered by the MIT license                        *
 *                                                                                   *
 *                        https://github.com/YumStudioHQ/YumEngine                   *
 *                             https://github.com/YumStudioHQ                        *
 *                              https://github.com/wys-prog                          *
 *                                                                                   *
 *************************************************************************************/

#pragma once

#include "enumerable.hpp"
#include "memoryslice.hpp"
#include "span.hpp"

namespace YumEngine::xV1::containers {
  template <typename CharT>
  class stringlookup : public memoryslice<CharT> {
  public:
    inline stringlookup() 
      :memoryslice<CharT>() {}
    inline stringlookup(const CharT *start, uint64_t s)
      : memoryslice<CharT>(start, s) {}

    list<stringlookup<CharT>> split(const CharT &del) const {
      list<stringlookup<CharT>> views;

      uint64_t last = 0;
      for (uint64_t i = 0; i < this->_length; i++) {
        if (this->start[i] == del) {
          views.append(stringlookup<CharT>(this->start + last, i - last));
          last = i + 1;
        }
      }

      if (last != this->_length) views.append(stringlookup<CharT>(this->start + last, this->_length - last));

      return views;
    }

    list<stringlookup<CharT>> split(const stringlookup<CharT> &del) const {
      list<stringlookup<CharT>> views;

      if (del._length == 0) {
        views.append(*this);
        return views;
      }

      uint64_t last = 0;
      for (uint64_t i = 0; i + del._length <= this->_length; i++) {

        bool match = true;
        for (uint64_t j = 0; j < del._length; j++) {
          if (this->start[i + j] != del.start[j]) {
            match = false;
            break;
          }
        }

        if (match) {
          views.append(stringlookup(this->start + last, i - last));
          i += del._length - 1;
          last = i + 1;
        }
      }

      if (last < this->_length) {
        views.append(stringlookup(this->start + last, this->_length - last));
      }

      return views;
    }

    template <typename Callback>
    void split(const CharT &del, Callback callback) const {
      uint64_t last = 0;
      for (uint64_t i = 0; i < this->_length; i++) {
        if (this->start[i] == del) {
          callback(stringlookup<CharT>(this->start + last, i - last));
          last = i + 1;
        }
      }

      if (last != this->_length) callback(stringlookup<CharT>(this->start + last, this->_length - last));
    }

    template <typename Callback>
    void split(const stringlookup<CharT> &del, Callback callback) const {

      if (del._length == 0) {
        callback(*this);
        return;
      }

      uint64_t last = 0;
      for (uint64_t i = 0; i + del._length <= this->_length; i++) {

        bool match = true;
        for (uint64_t j = 0; j < del._length; j++) {
          if (this->start[i + j] != del.start[j]) {
            match = false;
            break;
          }
        }

        if (match) {
          callback(stringlookup(this->start + last, i - last));
          i += del._length - 1;
          last = i + 1;
        }
      }

      if (last <= this->_length) {
        callback(stringlookup(this->start + last, this->_length - last));
      }
    }
  };

  template <typename CharT>
  class basic_string : public enumerable < basic_string<CharT>, CharT > {
  protected:
    CharT   *start;
    uint64_t capacity;
    uint64_t _size;

  public:
    basic_string() : start(nullptr), _size(0) {}
    basic_string(const CharT *source, uint64_t len) 
      : start(nullptr), capacity(len), _size(len) {
        start = new CharT[len];
        for (uint64_t i = 0; i < len; i++) start[i] = source[i];
      }
    
    ~basic_string() {
      if (start) delete[] start;
      start = nullptr;
      _size = 0;
      capacity = 0;
    }

    auto _enumerable_head_impl() const {
      return start;
    }

    void realloc(uint64_t size) {
      CharT *buff = new CharT[size];
      for (uint64_t i = 0; i < std::min(size, _size); i++) {
        buff[i] = start[i];
      }

      delete[] start;
      start = buff;
    }

    inline uint64_t length() const override { return _size; }

    template <typename Pred>
    auto _enumerable_where_impl(Pred pred) {
      list<CharT> result;
      for (uint64_t i = 0; i < _size; i++)
        if (pred(start[i])) result.push_back(start[i]);
      return result;
    }

    template <typename Mapper>
    auto _enumerable_select_impl(Mapper mapper) const {
      using U = decltype(mapper(std::declval<CharT>()));
      list<U> result;
      for (uint64_t i = 0; i < _size; i++)
        result.push_back(mapper(start[i]));
      return result;
    }

    template <typename Pred>
    auto _enumerable_all_impl(Pred pred) const {
      for (uint64_t i = 0; i < _size; i++)
        if (!pred(start[i])) return false;
      return true;
    }

    template <typename Pred>
    auto _enumerable_any_impl(Pred pred) const {
      for (uint64_t i = 0; i < _size; i++)
        if (pred(start[i])) return true;
      return false;
    }

    template <typename Iterator>
    auto _enumerable_iterator_impl(Iterator iterator) const {
      for (uint64_t i = 0; i < _size; i++) iterator(start[i]);
    }

    auto _enumerable_slice_impl(uint64_t start, uint64_t count) const {
      if (start >= this->size())
        throw std::out_of_range("Start index out of range");
      list<CharT> result;
      auto end = std::min(start + count, this->size());
      for (uint64_t i = start; i < end; ++i)
        result.push_back((*this)[i]);
      return result;
    }

    auto _enumerable_append_impl(const CharT &e) {
      if (_size + 1 > capacity) realloc(std::max((uint64_t)(capacity * 1.2f), _size + 1));
      start[_size] = e;
      _size++;
    }

    auto _enumerable_append_impl(const CharT *e, uint64_t len) {
      if (_size + len > capacity) realloc((uint64_t)(capacity * 1.1f) + len);
      for (uint64_t i = 0; i < len; i++) {
        start[_size + i] = e[i];
      }
      _size += len;
    }

    auto _enumerable_append_impl(const basic_string<CharT> &base) {
      _enumerable_append_impl(base.start, base._size);
    }

    auto _enumerable_append_impl(const basic_string<CharT> &base, uint64_t times) {
      for (uint64_t i = 0; i < times; i++)
        _enumerable_append_impl(base.start, base._size);
    }

    auto _enumerable_add_impl(const CharT &e) const {
      uint64_t newsize = _size + 1;
      CharT *buff = new CharT[newsize];

      for (uint64_t i = 0; i < _size; i++)
        buff[i] = start[i];

      buff[_size] = e;
      return memoryslice<CharT>(buff, newsize, true);
    }

    CharT &_enumerable_at(uint64_t index) {
      return start[index];
    }

    CharT &_enumerable_at_const(uint64_t index) const {
      return start[index];
    }

    auto _enumerable_contains_impl(const CharT &e) const {
      return _enumerable_indexof_impl(e) != _enumerable_end_impl();
    }
    
    auto _enumerable_indexof_impl(const CharT &e) const {
      for (uint64_t i = 0; i < length(); i++) {
        if (start[i] == e) return i;
      }

      return _enumerable_end_impl();
    }

    auto _enumerable_end_impl() const {
      return length() + 1;
    }

    CharT copy(uint64_t index) const {
      if (index >= length()) throw std::out_of_range("basic_string<CharT>.copy(): Invalid index");
      return start[index];
    }

    list<CharT> tolist() const {
      list<CharT> li;
      (*this).foreach([&li](auto e) {
        li.push_back(e);
      });

      return li;
    }

    memoryslice<CharT> duplicate() const {
      return memoryslice<CharT>(start, _size, true);
    }
  };
}

namespace YumEngine::xV1 {
  namespace Sdk {
    /**
     * @brief Guarantees that the type does not hold the string, and does not copy when creating it.
     * @tparam The character type.
     */
    template <typename CharT>
    using ownerless_basic_string = YumEngine::xV1::containers::stringlookup<CharT>;

    /**
     * @brief Provides a type that allows manipulating strings rapidely, without precising if it
     * holds the string or not.
     * @tparam The character type.
     */
    template <typename CharT>
    using stringlookup = YumEngine::xV1::containers::stringlookup<CharT>;
    
    /**
     * @brief A type that points to a string. It doesn't ensures if it holds the string or not.
     * @tparam The character type.
     */
    template <typename CharT>
    using str = YumEngine::xV1::containers::stringlookup<CharT>;

    /**
     * @brief Guarantees that the type does not hold the string, and does not copy when creating it.
     */
    using strview = YumEngine::xV1::containers::stringlookup<char>;
  }
}