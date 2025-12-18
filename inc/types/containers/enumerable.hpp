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

#include <vector>
#include <stdexcept>

/**
 * @namespace YumEngine::xV1::containers
 * @brief Contains container-related utilities and classes for the YumEngine framework.
 *
 * This namespace provides LINQ-like enumerable abstractions and concrete
 * container implementations used throughout YumEngine.
 */
namespace YumEngine::xV1::containers {

  /**
   * @class enumerable
   * @brief CRTP-based base class that provides enumerable operations.
   *
   * This class implements a LINQ-style interface using the
   * Curiously Recurring Template Pattern (CRTP).
   *
   * @tparam Derived The derived container type.
   * @tparam T       The element type.
   */
  template <typename Derived, typename T>
  class enumerable {
  public:
    /**
     * @brief Get a mutable reference to the derived instance.
     */
    Derived& derived() { return static_cast<Derived&>(*this); }

    /**
     * @brief Get a const reference to the derived instance.
     */
    const Derived& derived() const { return static_cast<const Derived&>(*this); }

    /**
     * @brief Get the number of elements.
     * @return Number of elements in the enumerable.
     *
     * This function is expected to be overridden by derived types.
     */
    inline virtual uint64_t length() const { return 0; }

    /**
     * @brief Get a pointer to the first element.
     */
    auto head() const { return derived()._enumerable_head_impl(); }

    /**
     * @brief Filter elements by predicate.
     * @tparam Pred Predicate type.
     * @param pred Predicate function.
     * @return A new enumerable containing elements that satisfy the predicate.
     */
    template <typename Pred>
    auto where(Pred pred) const {
      return derived()._enumerable_where_impl(pred);
    }

    /**
     * @brief Map elements to a new type.
     * @tparam Mapper Mapping function type.
     * @param mapper Mapping function.
     * @return A new enumerable with mapped elements.
     */
    template <typename Mapper>
    auto select(Mapper mapper) const {
      return derived()._enumerable_select_impl(mapper);
    }

    /**
     * @brief Check if all elements satisfy a predicate.
     * @tparam Pred Predicate type.
     * @param pred Predicate function.
     * @return True if all elements satisfy the predicate.
     */
    template <typename Pred>
    auto all(Pred pred) const {
      return derived()._enumerable_all_impl(pred);
    }

    /**
     * @brief Check if any element satisfies a predicate.
     * @tparam Pred Predicate type.
     * @param pred Predicate function.
     * @return True if any element satisfies the predicate.
     */
    template <typename Pred>
    auto any(Pred pred) const {
      return derived()._enumerable_any_impl(pred);
    }

    /**
     * @brief Iterate over each element.
     * @tparam Iterator Callable type.
     * @param iterator Function applied to each element.
     */
    template <typename Iterator>
    auto foreach(Iterator iterator) const {
      return derived()._enumerable_iterator_impl(iterator);
    }

    /**
     * @brief Create a slice of the enumerable.
     * @param start Starting index.
     * @param count Number of elements.
     * @return A new enumerable containing the slice.
     */
    auto slice(uint64_t start, uint64_t count) const {
      return derived()._enumerable_slice_impl(start, count);
    }

    /**
     * @brief Append an element to the underlying container.
     * @param e Element to append.
     */
    auto append(const T &e) {
      return derived()._enumerable_append_impl(e);
    }

    /**
     * @brief Create a new enumerable with an element added.
     * @param e Element to add.
     * @return A new enumerable containing the element.
     */
    auto add(const T &e) const {
      return derived()._enumerable_add_impl(e);
    }

    /**
     * @brief Access element at index with bounds checking.
     * @param index Element index.
     * @throws std::out_of_range If index is invalid.
     */
    auto at(uint64_t index) {
      if (index >= length()) throw std::out_of_range("Accessing out of range index in enumerable<Derived, T> type");
      return derived()._enumerable_at(index);
    }

    /**
     * @brief Access element at index with bounds checking (const).
     * @param index Element index.
     * @throws std::out_of_range If index is invalid.
     */
    auto at(uint64_t index) const {
      if (index >= length()) throw std::out_of_range("Accessing out of range index in enumerable<Derived, T> type");
      return derived()._enumerable_at_const(index);
    }

    /**
     * @brief Indexing operator (const).
     */
    auto operator[](uint64_t index) const {
      return at(index);
    }

    /**
     * @brief Indexing operator.
     */
    auto operator[](uint64_t index) {
      return at(index);
    }

    /**
     * @brief Check if the enumerable contains an element.
     * @param e Element to search for.
     * @return True if element exists.
     */
    auto contains(const T &e) const {
      return derived()._enumerable_contains_impl(e);
    }

    /**
     * @brief Get the index of an element.
     * @param e Element to search for.
     * @return Index of the element or end sentinel.
     */
    auto indexof(const T &e) const {
      return derived()._enumerable_indexof_impl(e);
    }

    /**
     * @brief Get the end sentinel value.
     */
    auto atend() const {
      return derived()._enumerable_end_impl();
    }

    /**
     * @brief Cast this enumerable to another enumerable type.
     * @tparam EnumerableT Target enumerable type.
     */
    template <typename EnumerableT>
    EnumerableT as() const {
      return EnumerableT(head(), length());
    }
    
    /**
     * @brief Cast this enumerable to another enumerable type with explicit cast.
     * @tparam EnumerableT Target enumerable type.
     * @tparam Cast        Cast type.
     */
    template <typename EnumerableT, typename Cast>
    EnumerableT as() const {
      return EnumerableT((const Cast*)head(), length());
    }

    virtual ~enumerable() {}
  };

  /**
   * @class list
   * @brief Concrete enumerable backed by std::vector.
   *
   * @tparam T Element type.
   */
  template <typename T>
  class list : public enumerable<list<T>, T>, public std::vector<T> {
  public:
    /**
     * @brief Default constructor.
     */
    inline list() {}

    /**
     * @brief Construct from std::vector.
     * @param vec Source vector.
     */
    inline list(const std::vector<T> &vec) 
      : std::vector<T>(vec) {}

    /**
     * @brief Construct from raw array.
     * @param beg Pointer to first element.
     * @param len Number of elements.
     */
    inline list(const T *beg, uint64_t len)
      : std::vector<T>() {
      for (uint64_t i = 0; i < len; i++) this->push_back(beg[i]);
    }

    /**
     * @brief Construct from an initializer list.
     * @param inili An initializer list.
     */
    inline list(const std::initializer_list<T> &inili) 
      : std::vector<T>(inili) {}

    /**
     * @brief Implementation of where().
     */
    template <typename Pred>
    auto _enumerable_where_impl(Pred pred) const {
      list<T> result;
      for (auto &x : *this)
        if (pred(x)) result.push_back(x);
      return result;
    }

    /**
     * @brief Implementation of select().
     */
    template <typename Mapper>
    auto _enumerable_select_impl(Mapper mapper) const {
      using U = decltype(mapper(std::declval<T>()));
      list<U> result;
      for (auto &x : *this)
        result.push_back(mapper(x));
      return result;
    }

    /**
     * @brief Implementation of all().
     */
    template <typename Pred>
    auto _enumerable_all_impl(Pred pred) const {
      for (auto &x : *this)
        if (!pred(x)) return false;
      return true;
    }

    /**
     * @brief Implementation of any().
     */
    template <typename Pred>
    auto _enumerable_any_impl(Pred pred) const {
      for (auto &x : *this)
        if (pred(x)) return true;
      return false;
    }

    /**
     * @brief Implementation of foreach().
     */
    template <typename Iterator>
    auto _enumerable_iterator_impl(Iterator iterator) const {
      for (auto &x : *this) iterator(x);
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
     * @brief Implementation of head().
     */
    auto _enumerable_head_impl() const {
      return &this->front();
    }

    /**
     * @brief Implementation of append().
     */
    auto _enumerable_append_impl(const T &e) {
      return this->push_back(e);
    }

    /**
     * @brief Implementation of add().
     */
    auto _enumerable_add_impl(const T &e) const {
      list<T> second = *this;
      second.push_back(e);
      return second;
    }

    /**
     * @brief Implementation of at().
     */
    auto _enumerable_at(uint64_t index) {
      return this->data()[index];
    }

    /**
     * @brief Implementation of at() const.
     */
    auto _enumerable_at_const(uint64_t index) const {
      return this->data()[index];
    }

    /**
     * @brief Implementation of contains().
     */
    auto _enumerable_contains_impl(const T &e) const {
      return this->contains(e);
    }
    
    /**
     * @brief Implementation of indexof().
     */
    auto _enumerable_indexof_impl(const T &e) const {
      for (uint64_t i = 0; i < length(); i++) {
        if ((*this)[i] == e) return i;
      }

      return _enumerable_end_impl();
    }

    /**
     * @brief Implementation of end sentinel.
     */
    auto _enumerable_end_impl() const {
      return length() + 1;
    }

    /**
     * @brief Get the number of elements.
     */
    inline uint64_t length() const override { return this->size(); }

    /**
     * @brief Join multiple lists into one.
     * @param lists List of lists to join.
     * @return A new list containing all elements.
     */
    list<T> join(const list<list<T>> lists) const {
      list<T> copy = (*this);
      for (const auto &listel : lists) {
        for (const auto &element : listel) {
          copy.append(element);
        }
      }

      return copy;
    }

    /**
     * @brief Convert elements to a new type using constructor.
     * @tparam NewType Target type.
     */
    template <typename NewType>
    list<NewType> convert() const {
      list<NewType> vec = {};
      for (const auto &x : (*this)) vec.push_back(NewType(x));
      return vec;
    }

    /**
     * @brief Convert elements to a new type using cast.
     * @tparam NewType Target type.
     * @tparam Cast    Cast type.
     */
    template <typename NewType, typename Cast>
    list<NewType> convert() const {
      list<NewType> vec = {};
      for (const auto &x : (*this)) vec.push_back((Cast)x);
      return vec;
    }

    /**
     * @brief Convert elements to a new type using a cast function.
     * @tparam NewType      Target type.
     * @tparam CastFunction Conversion function.
     */
    template <typename NewType, auto CastFunction>
    list<NewType> convert() const {
      list<NewType> vec = {};
      for (const auto &x : (*this)) vec.push_back(CastFunction(x));
      return vec;
    }
  };
}
