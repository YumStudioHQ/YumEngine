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

namespace YumEngine::xV1::containers {
  template <typename T>
  class span : public enumerable < span<T>, T > {
  private:
    T *start;
    uint64_t _length;

  public:
    inline span() : start(nullptr), _length(0) { }
    inline span(const T *beg, uint64_t len)
      : start(nullptr), _length(len) {
        start = new T[len];
        for (uint64_t i = 0; i < len; i++) {
          start[i] = beg[i];
        }
      }

    template <typename Derived>
    inline span(const enumerable<Derived, T> &en) {
      start = new T[en.length()];
      _length = en.length();
      for (uint64_t i = 0; i < en.length(); i++) start[i] = en[i];
    }

    inline ~span() {
      delete[] start;
    }

    inline uint64_t length() const override { return _length; }

    auto _enumerable_head_impl() const {
      return start;
    }

    template <typename Pred>
    auto _enumerable_where_impl(Pred pred) const {
      list<T> result;
      for (uint64_t i = 0; i < _length; i++)
        if (pred(start[i])) result.push_back(start[i]);
      return result;
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

    auto _enumerable_append_impl(const T &e) const {
      list<T> tmp = tolist();
      tmp.append(e);
      return tmp;
    }

    auto _enumerable_add_impl(const T &e) const {
      return _enumerable_append_impl(e);
    }

    T &_enumerable_at(uint64_t index) {
      return start[index];
    }

    const T &_enumerable_at_const(uint64_t index) const {
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

    list<T> tolist() const {
      list<T> li;
      (*this).foreach([&li](auto e) {
        li.push_back(e);
      });

      return li;
    }

    memoryslice<T> tomemory() const {
      return memoryslice<T>(start, _length, false);
    }

    memoryslice<T> duplicate() const {
      return memoryslice<T>(start, _length, true);
    }
  };

}