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

  /**
   * @class span
   * @brief Owning contiguous container compatible with enumerable.
   *
   * span represents a contiguous block of memory that is always owned
   * by the instance. It can be constructed from raw memory or any
   * enumerable-compatible container.
   *
   * @tparam T Element type.
   */
  template <typename T>
  class span : public enumerable < span<T>, T > {
  private:
    /** Pointer to the beginning of the memory block */
    T *start;

    /** Number of elements */
    uint64_t _length;

  public:
    /**
     * @brief Default constructor.
     *
     * Creates an empty span.
     */
    inline span() : start(nullptr), _length(0) { }

    /**
     * @brief Construct span by copying raw memory.
     *
     * @param beg Pointer to source memory.
     * @param len Number of elements.
     */
    inline span(const T *beg, uint64_t len)
      : start(nullptr), _length(len) {
        start = new T[len];
        for (uint64_t i = 0; i < len; i++) {
          start[i] = beg[i];
        }
      }

    /**
     * @brief Construct span with `len` empty elements.
     * 
     * @param len Size of the span.
     */
    inline span(uint64_t len) {
      start = new T[len]();
      _length = len;
    }

    /**
     * @brief Construct span from an enumerable.
     *
     * Copies all elements from the given enumerable into
     * owned contiguous storage.
     *
     * @tparam Derived Enumerable derived type.
     * @param en Source enumerable.
     */
    template <typename Derived>
    inline span(const enumerable<Derived, T> &en) {
      start = new T[en.length()];
      _length = en.length();
      for (uint64_t i = 0; i < en.length(); i++) start[i] = en[i];
    }

    /**
     * @brief Destructor.
     *
     * Frees owned memory.
     */
    inline ~span() {
      delete[] start;
    }

    /**
     * @brief Get the number of elements.
     */
    inline uint64_t length() const override { return _length; }

    /**
     * @brief Implementation of head().
     */
    auto _enumerable_head_impl() const {
      return start;
    }

    /**
     * @brief Implementation of where().
     */
    template <typename Pred>
    auto _enumerable_where_impl(Pred pred) const {
      list<T> result;
      for (uint64_t i = 0; i < _length; i++)
        if (pred(start[i])) result.push_back(start[i]);
      return result;
    }

    /**
     * @brief Implementation of select().
     */
    template <typename Mapper>
    auto _enumerable_select_impl(Mapper mapper) const {
      using U = decltype(mapper(std::declval<T>()));
      list<U> result;
      for (uint64_t i = 0; i < _length; i++)
        result.push_back(mapper(start[i]));
      return result;
    }

    /**
     * @brief Implementation of all().
     */
    template <typename Pred>
    auto _enumerable_all_impl(Pred pred) const {
      for (uint64_t i = 0; i < _length; i++)
        if (!pred(start[i])) return false;
      return true;
    }

    /**
     * @brief Implementation of any().
     */
    template <typename Pred>
    auto _enumerable_any_impl(Pred pred) const {
      for (uint64_t i = 0; i < _length; i++)
        if (pred(start[i])) return true;
      return false;
    }

    /**
     * @brief Implementation of foreach().
     */
    template <typename Iterator>
    auto _enumerable_iterator_impl(Iterator iterator) const {
      for (uint64_t i = 0; i < _length; i++) iterator(start[i]);
    }

    /**
     * @brief Implementation of slice().
     */
    auto _enumerable_slice_impl(uint64_t start, uint64_t count) const {
      if (start >= this->size())
        throw std::out_of_range("Start index out of range");
      list<T> result;
      auto end = std::min(start + count, this->size());
      for (uint64_t i = start; i < end; ++i)
        result.push_back((*this)[i]);
      return result;
    }

    /**
     * @brief Append an element (returns a new list).
     *
     * span itself is immutable in size; this operation
     * returns a list containing the appended element.
     */
    auto _enumerable_append_impl(const T &e) const {
      list<T> tmp = tolist();
      tmp.append(e);
      return tmp;
    }

    /**
     * @brief Add an element (alias of append).
     */
    auto _enumerable_add_impl(const T &e) const {
      return _enumerable_append_impl(e);
    }

    /**
     * @brief Mutable element access.
     */
    T &_enumerable_at(uint64_t index) {
      return start[index];
    }

    /**
     * @brief Const element access.
     */
    const T &_enumerable_at_const(uint64_t index) const {
      return start[index];
    }

    /**
     * @brief Implementation of contains().
     */
    auto _enumerable_contains_impl(const T &e) const {
      return _enumerable_indexof_impl(e) != _enumerable_end_impl();
    }
    
    /**
     * @brief Implementation of indexof().
     */
    auto _enumerable_indexof_impl(const T &e) const {
      for (uint64_t i = 0; i < length(); i++) {
        if (start[i] == e) return i;
      }

      return _enumerable_end_impl();
    }

    /**
     * @brief End sentinel value.
     */
    auto _enumerable_end_impl() const {
      return length() + 1;
    }

    /**
     * @brief Convert this span to a list.
     */
    list<T> tolist() const {
      list<T> li;
      (*this).foreach([&li](auto e) {
        li.push_back(e);
      });

      return li;
    }

    /**
     * @brief Create a non-owning memoryslice view.
     */
    memoryslice<T> tomemory() const {
      return memoryslice<T>(start, _length, false);
    }

    /**
     * @brief Create an owning memoryslice copy.
     */
    memoryslice<T> duplicate() const {
      return memoryslice<T>(start, _length, true);
    }
  };

}
