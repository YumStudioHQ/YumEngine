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
#include <cstdint>

namespace YumEngine::xV1::containers {
  template <typename T>
  class memoryslice : public enumerable < memoryslice<T>, T > {
  protected:
    T       *start;
    uint64_t _length;
    bool     owns = false;
    bool     readonly = false;
    
  public:
    inline memoryslice() 
      : start(nullptr), _length(0), owns(false)
      {}

    inline memoryslice(const T *start, uint64_t size, bool copy = false) {
      if (copy) {
        this->start = new T[size];
        this->_length = size;
        this->owns = true;
        std::copy(start, start + size, const_cast<T*>(this->start));
      } else {
        this->start = const_cast<T*>(start);
        this->_length = size;
        this->owns = false;
        readonly = true;
      }
    }

    inline memoryslice(const T *start, uint64_t size, uint64_t newsize) {
      this->start = new T[newsize];
      this->_length = newsize;
      this->owns = true;
      std::copy(start, start + std::min(newsize, size), const_cast<T*>(this->start));
    }


    inline memoryslice(uint64_t size) {
      this->start = new T[size];
      this->_length = size;
      this->owns = true;
    }

    inline memoryslice(const memoryslice<T> &from) {
      this->start = new T[from._length];
      this->_length = from._length;
      this->owns = true;
      std::copy(from.start, from.start + from._length, const_cast<T*>(this->start));
    }

    inline memoryslice(const memoryslice<T> &from, uint64_t newsize) {
      this->start = new T[from._length];
      this->_length = newsize;
      this->owns = true;
      std::copy(from.start, from.start + std::min(newsize, from._length), const_cast<T*>(this->start));
    }

    inline ~memoryslice() {
      if (owns) {
        delete[] start;
      }
    }

    inline uint64_t length() const override { return _length; }

    template <typename Pred>
    auto _enumerable_where_impl(Pred pred) const {
      list<T> result;
      for (uint64_t i = 0; i < _length; i++)
        if (pred(start[i])) result.push_back(start[i]);
      return result;
    }

    auto _enumerable_head_impl() const {
      return start;
    }

    template <typename Mapper>
    auto _enumerable_select_impl(Mapper mapper) const {
      using U = decltype(mapper(std::declval<T>()));
      list<U> result;
      for (uint64_t i = 0; i < _length; i++)
        result.push_back(mapper(start[i]));
      return result;
    }

    template <typename Pred>
    auto _enumerable_all_impl(Pred pred) const {
      for (uint64_t i = 0; i < _length; i++)
        if (!pred(start[i])) return false;
      return true;
    }

    template <typename Pred>
    auto _enumerable_any_impl(Pred pred) const {
      for (uint64_t i = 0; i < _length; i++)
        if (pred(start[i])) return true;
      return false;
    }

    template <typename Iterator>
    auto _enumerable_iterator_impl(Iterator iterator) const {
      for (uint64_t i = 0; i < _length; i++) iterator(start[i]);
    }

    auto _enumerable_slice_impl(uint64_t start, uint64_t count) const {
      if (start >= this->size())
        throw std::out_of_range("Start index out of range");
      list<T> result;
      auto end = std::min(start + count, this->size());
      for (uint64_t i = start; i < end; ++i)
        result.push_back((*this)[i]);
      return result;
    }

    memoryslice<T> &_enumerable_append_impl(const T &e) {
      uint64_t newsize = _length + 1;
      T *buff = new T[newsize];

      for (uint64_t i = 0; i < _length; i++)
        buff[i] = start[i];

      buff[_length] = e;

      if (owns) delete[] start;

      start = buff;
      _length = newsize;
      owns = true;
      return *this;
    }

    auto _enumerable_add_impl(const T &e) const {
      uint64_t newsize = _length + 1;
      T *buff = new T[newsize];

      for (uint64_t i = 0; i < _length; i++)
        buff[i] = start[i];

      buff[_length] = e;
      return memoryslice<T>(buff, newsize, true);
    }

    T &_enumerable_at(uint64_t index) {
      if (readonly)
        throw std::logic_error("attempt to modify read-only memoryslice");
      return start[index];
    }

    T &_enumerable_at_const(uint64_t index) const {
      return start[index];
    }

    auto _enumerable_contains_impl(const T &e) const {
      return _enumerable_indexof_impl(e) != _enumerable_end_impl();
    }
    
    auto _enumerable_indexof_impl(const T &e) const {
      for (uint64_t i = 0; i < length(); i++) {
        if (start[i] == e) return i;
      }

      return _enumerable_end_impl();
    }

    auto _enumerable_end_impl() const {
      return length() + 1;
    }

    T copy(uint64_t index) const {
      if (index >= length()) throw std::out_of_range("memoryslice<T>.copy(): Invalid index");
      return start[index];
    }

    list<T> tolist() const {
      list<T> li;
      (*this).foreach([&li](auto e) {
        li.push_back(e);
      });

      return li;
    }

    memoryslice<T> duplicate() const {
      return memoryslice<T>(start, _length, true);
    }

    inline bool owns_memory() const { return owns; }
  };
}