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

  /**
   * @class memoryslice
   * @brief Lightweight non-owning or owning view over a contiguous memory region.
   *
   * memoryslice provides an enumerable-compatible abstraction over raw memory.
   * It may either own the underlying memory or reference external memory,
   * and can optionally be read-only.
   *
   * @tparam T Element type.
   */
  template <typename T>
  class memoryslice : public enumerable < memoryslice<T>, T > {
  protected:
    /** Pointer to the beginning of the memory region */
    T       *start;

    /** Number of elements in the slice */
    uint64_t _length;

    /** Whether this slice owns the memory */
    bool     owns = false;

    /** Whether this slice is read-only */
    bool     readonly = false;
    
  public:
    /**
     * @brief Default constructor.
     *
     * Creates an empty, read-only memory slice.
     */
    inline memoryslice() 
      : start(nullptr), _length(0), owns(false), readonly(true)
      {}

    /**
     * @brief Construct from raw memory.
     *
     * @param start Pointer to source memory.
     * @param size  Number of elements.
     * @param copy  If true, the memory is copied and owned.
     */
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

    /**
     * @brief Construct from raw memory with resizing.
     *
     * @param start   Pointer to source memory.
     * @param size    Original size.
     * @param newsize New size of the slice.
     */
    inline memoryslice(const T *start, uint64_t size, uint64_t newsize) {
      this->start = new T[newsize];
      this->_length = newsize;
      this->owns = true;
      std::copy(start, start + std::min(newsize, size), const_cast<T*>(this->start));
    }

    /**
     * @brief Allocate a new memory slice of given size.
     *
     * @param size Number of elements.
     */
    inline memoryslice(uint64_t size) {
      this->start = new T[size];
      this->_length = size;
      this->owns = true;
    }

    /**
     * @brief Copy constructor.
     *
     * Creates a deep copy of another memoryslice.
     *
     * @param from Source slice.
     */
    inline memoryslice(const memoryslice<T> &from) {
      this->start = new T[from._length];
      this->_length = from._length;
      this->owns = true;
      std::copy(from.start, from.start + from._length, const_cast<T*>(this->start));
    }

    /**
     * @brief Copy constructor with resizing.
     *
     * @param from    Source slice.
     * @param newsize New size of the slice.
     */
    inline memoryslice(const memoryslice<T> &from, uint64_t newsize) {
      this->start = new T[from._length];
      this->_length = newsize;
      this->owns = true;
      std::copy(from.start, from.start + std::min(newsize, from._length), const_cast<T*>(this->start));
    }

    /**
     * @brief Destructor.
     *
     * Frees owned memory if applicable.
     */
    inline ~memoryslice() {
      if (owns) {
        delete[] start;
      }
    }

    /**
     * @brief Get the number of elements.
     */
    inline uint64_t length() const override { return _length; }

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
     * @brief Implementation of head().
     */
    auto _enumerable_head_impl() const {
      return start;
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
      if (start >= this->_length)
        throw std::out_of_range("Start index out of range");
      list<T> result;
      auto end = std::min(start + count, this->_length);
      for (uint64_t i = start; i < end; ++i)
        result.push_back((*this)[i]);
      return result;
    }

    /**
     * @brief Append an element to this slice (mutating).
     *
     * Reallocates memory and takes ownership.
     */
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

    /**
     * @brief Add an element and return a new slice.
     */
    auto _enumerable_add_impl(const T &e) const {
      uint64_t newsize = _length + 1;
      T *buff = new T[newsize];

      for (uint64_t i = 0; i < _length; i++)
        buff[i] = start[i];

      buff[_length] = e;
      return memoryslice<T>(buff, newsize, true);
    }

    /**
     * @brief Mutable element access.
     *
     * @throws std::logic_error If the slice is read-only.
     */
    T &_enumerable_at(uint64_t index) {
      if (readonly)
        throw std::logic_error("attempt to modify read-only memoryslice");
      return start[index];
    }

    /**
     * @brief Const element access.
     */
    T &_enumerable_at_const(uint64_t index) const {
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
     * @brief Copy an element by value.
     *
     * @param index Element index.
     * @throws std::out_of_range If index is invalid.
     */
    T copy(uint64_t index) const {
      if (index >= length()) throw std::out_of_range("memoryslice<T>.copy(): Invalid index");
      return start[index];
    }

    /**
     * @brief Convert this slice to a list.
     */
    list<T> tolist() const {
      list<T> li;
      (*this).foreach([&li](auto e) {
        li.push_back(e);
      });

      return li;
    }

    /**
     * @brief Create a deep copy of this slice.
     */
    memoryslice<T> duplicate() const {
      return memoryslice<T>(start, _length, true);
    }

    /**
     * @brief Check if this slice owns its memory.
     */
    inline bool owns_memory() const { return owns; }
  };
}
