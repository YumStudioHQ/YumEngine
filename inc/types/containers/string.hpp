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

#include <string>

namespace YumEngine::xV1::containers {

  /**
   * @class stringlookup
   * @brief Non-owning string view with split utilities.
   *
   * stringlookup is a lightweight wrapper over memoryslice, designed
   * specifically for string-like data. It never owns the underlying
   * memory unless explicitly copied elsewhere.
   *
   * @tparam CharT Character type.
   */
  template <typename CharT>
  class stringlookup : public memoryslice<CharT> {
  public:
    /**
     * @brief Default constructor.
     *
     * Creates an empty string lookup.
     */
    inline stringlookup() 
      :memoryslice<CharT>() {}

    /**
     * @brief Construct from raw character buffer.
     *
     * @param start Pointer to character data.
     * @param s     Length of the string.
     */
    inline stringlookup(const CharT *start, uint64_t s)
      : memoryslice<CharT>(start, s) {}

    /**
     * @brief Split the string using a single character delimiter.
     *
     * @param del Delimiter character.
     * @return List of stringlookup views.
     */
    list<stringlookup<CharT>> split(const CharT &del) const {
      list<stringlookup<CharT>> views;

      uint64_t last = 0;
      for (uint64_t i = 0; i < this->_length; i++) {
        if (this->start[i] == del) {
          views.append(stringlookup<CharT>(this->start + last, i - last));
          last = i + 1;
        }
      }

      if (last != this->_length)
        views.append(stringlookup<CharT>(this->start + last, this->_length - last));

      return views;
    }

    /**
     * @brief Split the string using another string as delimiter.
     *
     * @param del Delimiter string.
     * @return List of stringlookup views.
     */
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
          views.append(stringlookup<CharT>(this->start + last, i - last));
          i += del._length - 1;
          last = i + 1;
        }
      }

      if (last < this->_length) {
        views.append(stringlookup(this->start + last, this->_length - last));
      }

      return views;
    }

    /**
     * @brief Split the string using a character delimiter and invoke a callback.
     *
     * @tparam Callback Callable type.
     * @param del      Delimiter character.
     * @param callback Function called for each substring.
     */
    template <typename Callback>
    void split(const CharT &del, Callback callback) const {
      uint64_t last = 0;
      for (uint64_t i = 0; i < this->_length; i++) {
        if (this->start[i] == del) {
          callback(stringlookup<CharT>(this->start + last, i - last));
          last = i + 1;
        }
      }

      if (last != this->_length)
        callback(stringlookup<CharT>(this->start + last, this->_length - last));
    }

    /**
     * @brief Split the string using a string delimiter and invoke a callback.
     *
     * @tparam Callback Callable type.
     * @param del      Delimiter string.
     * @param callback Function called for each substring.
     */
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

      if (last < this->_length) {
        callback(stringlookup(this->start + last, this->_length - last));
      }
    }
  };

  /**
   * @class basic_string
   * @brief Owning, mutable string container with enumerable support.
   *
   * basic_string manages its own memory and provides dynamic growth,
   * enumerable operations, and interoperability with stringlookup
   * and memoryslice.
   *
   * @tparam CharT Character type.
   */
  template <typename CharT>
  class basic_string : public enumerable < basic_string<CharT>, CharT > {
  protected:
    /** Pointer to owned character buffer */
    CharT   *start = nullptr;

    /** Allocated capacity */
    uint64_t capacity = 0;

    /** Current string size */
    uint64_t _size = 0;

  public:
    /**
     * @brief Default constructor.
     */
    basic_string() : start(nullptr), _size(0) {}

    /**
     * @brief Construct from raw character buffer.
     *
     * @param source Pointer to character data.
     * @param len    Number of characters.
     */
    basic_string(const CharT *source, uint64_t len) 
      : start(nullptr), capacity(len), _size(len) {
        start = new CharT[len];
        for (uint64_t i = 0; i < len; i++) start[i] = source[i];
      }
    
    /**
     * @brief Construct from a stringlookup.
     *
     * Performs a deep copy.
     *
     * @param lookup Source string view.
     */
    basic_string(const stringlookup<CharT> &lookup) 
      : start(nullptr), capacity(lookup.length()), _size(lookup.length()) {
        start = new CharT[lookup.length()];
        for (uint64_t i = 0; i < lookup.length(); i++) start[i] = lookup[i];
      }

    basic_string(const basic_string& other)
      : start(nullptr), capacity(other._size), _size(other._size)
    {
      if (_size) {
        start = new CharT[_size];
        std::copy(other.start, other.start + _size, start);
      }
    }

    basic_string& operator=(const basic_string& other) {
      if (this == &other) return *this;

      delete[] start;
      start = nullptr;

      _size = other._size;
      capacity = other._size;

      if (_size) {
        start = new CharT[_size];
        std::copy(other.start, other.start + _size, start);
      }
      return *this;
    }

    inline basic_string(basic_string&& other) noexcept
      : start(other.start), capacity(other.capacity), _size(other._size)
    {
      other.start = nullptr;
      other.capacity = 0;
      other._size = 0;
    }

    basic_string& operator=(basic_string&& other) noexcept {
      if (this == &other) return *this;

      delete[] start;

      start = other.start;
      capacity = other.capacity;
      _size = other._size;

      other.start = nullptr;
      other.capacity = 0;
      other._size = 0;

      return *this;
    }


    /**
     * @brief Destructor.
     */
    ~basic_string() {
      if (start && capacity) delete[] start;
      start = nullptr;
      _size = 0;
      capacity = 0;
    }

    /**
     * @brief Implementation of head().
     */
    auto _enumerable_head_impl() const {
      return start;
    }

    /**
     * @brief Reallocate internal buffer.
     *
     * @param size New capacity.
     */
    void realloc(uint64_t size) {
      CharT* buff = new CharT[size];
      uint64_t newSize = std::min(size, _size);

      if (start) {
        for (uint64_t i = 0; i < newSize; i++)
          buff[i] = start[i];
        
        delete[] start;
      }

      start = buff;
      capacity = size;
      _size = newSize;
    }

    /**
     * @brief Get the string length.
     */
    inline uint64_t length() const override { return _size; }

    /**
     * @brief Implementation of where().
     */
    template <typename Pred>
    auto _enumerable_where_impl(Pred pred) {
      list<CharT> result;
      for (uint64_t i = 0; i < _size; i++)
        if (pred(start[i])) result.push_back(start[i]);
      return result;
    }

    /**
     * @brief Implementation of select().
     */
    template <typename Mapper>
    auto _enumerable_select_impl(Mapper mapper) const {
      using U = decltype(mapper(std::declval<CharT>()));
      list<U> result;
      for (uint64_t i = 0; i < _size; i++)
        result.push_back(mapper(start[i]));
      return result;
    }

    /**
     * @brief Implementation of all().
     */
    template <typename Pred>
    auto _enumerable_all_impl(Pred pred) const {
      for (uint64_t i = 0; i < _size; i++)
        if (!pred(start[i])) return false;
      return true;
    }

    /**
     * @brief Implementation of any().
     */
    template <typename Pred>
    auto _enumerable_any_impl(Pred pred) const {
      for (uint64_t i = 0; i < _size; i++)
        if (pred(start[i])) return true;
      return false;
    }

    /**
     * @brief Implementation of foreach().
     */
    template <typename Iterator>
    auto _enumerable_iterator_impl(Iterator iterator) const {
      for (uint64_t i = 0; i < _size; i++) iterator(start[i]);
    }

    /**
     * @brief Implementation of slice().
     */
    auto _enumerable_slice_impl(uint64_t start, uint64_t count) const {
      if (start >= this->size())
        throw std::out_of_range("Start index out of range");
      list<CharT> result;
      auto end = std::min(start + count, this->size());
      for (uint64_t i = start; i < end; ++i)
        result.push_back((*this)[i]);
      return result;
    }

    /**
     * @brief Append a character.
     */
    auto _enumerable_append_impl(const CharT &e) {
      if (_size + 1 > capacity)
        realloc(std::max((uint64_t)(capacity * 1.2f), _size + 1));
      start[_size] = e;
      _size++;
    }

    /**
     * @brief Append raw character data.
     */
    auto _enumerable_append_impl(const CharT *e, uint64_t len) {
      if (_size + len > capacity)
        realloc((uint64_t)(capacity * 1.1f) + len);
      for (uint64_t i = 0; i < len; i++) {
        start[_size + i] = e[i];
      }
      _size += len;
    }

    /**
     * @brief Append another basic_string.
     */
    auto _enumerable_append_impl(const basic_string<CharT> &base) {
      _enumerable_append_impl(base.start, base._size);
    }

    /**
     * @brief Append another string multiple times.
     */
    auto _enumerable_append_impl(const basic_string<CharT> &base, uint64_t times) {
      for (uint64_t i = 0; i < times; i++)
        _enumerable_append_impl(base.start, base._size);
    }

    /**
     * @brief Add a character and return a new slice.
     */
    auto _enumerable_add_impl(const CharT &e) const {
      uint64_t newsize = _size + 1;
      CharT *buff = new CharT[newsize];

      for (uint64_t i = 0; i < _size; i++)
        buff[i] = start[i];

      buff[_size] = e;
      auto slice = memoryslice<CharT>(buff, newsize, true);
      delete[] buff;
      return slice;
    }

    /**
     * @brief Mutable character access.
     */
    CharT &_enumerable_at(uint64_t index) {
      if (index >= _size)
        throw std::out_of_range("index");
      return start[index];
    }

    /**
     * @brief Const character access.
     */
    CharT &_enumerable_at_const(uint64_t index) const {
      if (index >= _size)
        throw std::out_of_range("index");
      return start[index];
    }

    /**
     * @brief Implementation of contains().
     */
    auto _enumerable_contains_impl(const CharT &e) const {
      return _enumerable_indexof_impl(e) != _enumerable_end_impl();
    }
    
    /**
     * @brief Implementation of indexof().
     */
    auto _enumerable_indexof_impl(const CharT &e) const {
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
     * @brief Copy a character by value.
     */
    CharT copy(uint64_t index) const {
      if (index >= length())
        throw std::out_of_range("basic_string<CharT>.copy(): Invalid index");
      return start[index];
    }

    /**
     * @brief Convert to a list of characters.
     */
    list<CharT> tolist() const {
      list<CharT> li;
      (*this).foreach([&li](auto e) {
        li.push_back(e);
      });

      return li;
    }

    /**
     * @brief Convert to std::basic_string.
     */
    std::basic_string<CharT> tostdstring() const {
      return std::basic_string<CharT>(this->start, this->_size);
    }

    /**
     * @brief Create an owning copy as a memoryslice.
     */
    memoryslice<CharT> duplicate() const {
      return memoryslice<CharT>(start, _size, true);
    }

    /**
     * @brief Append raw character data.
     */
    auto append(const CharT *e, uint64_t len) {
      return this->_enumerable_append_impl(e, len);
    }

    /**
     * @brief Append single char.
     */
    auto append(const CharT e) {
      return this->_enumerable_append_impl(e);
    }
    
    /**
     * @brief Append another string.
     */
    auto append(const basic_string<CharT> &base) {
      return this->_enumerable_append_impl(base);
    }

    /**
     * @brief Append another string multiple times.
     */
    auto append(const basic_string<CharT> &base, uint64_t times) {
      return this->_enumerable_append_impl(base, times);
    }
  };
}

/**
 * @namespace YumEngine::xV1::Sdk
 * @brief SDK-facing string type aliases.
 */
namespace YumEngine::xV1 {
  namespace Sdk {

    /**
     * @brief Non-owning string view alias.
     *
     * Guarantees that the type does not hold or copy the string.
     */
    template <typename CharT>
    using ownerless_basic_string = YumEngine::xV1::containers::stringlookup<CharT>;

    /**
     * @brief Flexible string view alias.
     *
     * May or may not own memory depending on source.
     */
    template <typename CharT>
    using stringlookup = YumEngine::xV1::containers::stringlookup<CharT>;
    
    /**
     * @brief Owning mutable string type.
     */
    template <typename CharT>
    using str = YumEngine::xV1::containers::basic_string<CharT>;

    /**
     * @brief Non-owning string view for char.
     */
    using strview = YumEngine::xV1::containers::stringlookup<char>;

    /**
     * @brief Alias for those who love SCREAMING_CASE.
     */
    template <typename CHAR_TYPE>
    using STRING = YumEngine::xV1::containers::basic_string<CHAR_TYPE>;

    /**
     * @brief Alias for those who love SCREAMING_CASE.
     */
    template <typename CHAR_TYPE>
    using STRING_VIEW = YumEngine::xV1::containers::stringlookup<CHAR_TYPE>;
  }
}
