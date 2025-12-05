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
 */

namespace YumEngine::xV1::containers {

  template <typename Derived, typename T>
  class enumerable {
  public:
    Derived& derived() { return static_cast<Derived&>(*this); }
    const Derived& derived() const { return static_cast<const Derived&>(*this); }

    inline virtual uint64_t length() const { return 0; }

    auto head() const { return derived()._enumerable_head_impl(); }

    template <typename Pred>
    auto where(Pred pred) const {
      return derived()._enumerable_where_impl(pred);
    }

    template <typename Mapper>
    auto select(Mapper mapper) const {
      return derived()._enumerable_select_impl(mapper);
    }

    template <typename Pred>
    auto all(Pred pred) const {
      return derived()._enumerable_all_impl(pred);
    }

    template <typename Pred>
    auto any(Pred pred) const {
      return derived()._enumerable_any_impl(pred);
    }

    template <typename Iterator>
    auto foreach(Iterator iterator) const {
      return derived()._enumerable_iterator_impl(iterator);
    }

    auto slice(uint64_t start, uint64_t count) const {
      return derived()._enumerable_slice_impl(start, count);
    }

    auto append(const T &e) {
      return derived()._enumerable_append_impl(e);
    }

    auto add(const T &e) const {
      return derived()._enumerable_add_impl(e);
    }

    auto at(uint64_t index) {
      if (index >= length()) throw std::out_of_range("Accessing out of range index in enumerable<Derived, T> type");
      return derived()._enumerable_at(index);
    }

    auto at(uint64_t index) const {
      if (index >= length()) throw std::out_of_range("Accessing out of range index in enumerable<Derived, T> type");
      return derived()._enumerable_at_const(index);
    }

    auto operator[](uint64_t index) const {
      return at(index);
    }

    auto operator[](uint64_t index) {
      return at(index);
    }

    auto contains(const T &e) const {
      return derived()._enumerable_contains_impl(e);
    }

    auto indexof(const T &e) const {
      return derived()._enumerable_indexof_impl(e);
    }

    auto atend() const {
      return derived()._enumerable_end_impl();
    }

    template <typename EnumerableT>
    EnumerableT as() const {
      return EnumerableT(head(), length());
    }
    
    template <typename EnumerableT, typename Cast>
    EnumerableT as() const {
      return EnumerableT((const Cast*)head(), length());
    }
  };

  template <typename T>
  class list : public enumerable<list<T>, T>, public std::vector<T> {
  public:
    inline list() {}
    inline list(const std::vector<T> &vec) 
      : std::vector<T>(vec) {}

    inline list(const T *beg, uint64_t len)
      : std::vector<T>() {
      for (uint64_t i = 0; i < len; i++) this->push_back(beg[i]);
    }

    template <typename Pred>
    auto _enumerable_where_impl(Pred pred) const {
      list<T> result;
      for (auto &x : *this)
        if (pred(x)) result.push_back(x);
      return result;
    }

    template <typename Mapper>
    auto _enumerable_select_impl(Mapper mapper) const {
      using U = decltype(mapper(std::declval<T>()));
      list<U> result;
      for (auto &x : *this)
        result.push_back(mapper(x));
      return result;
    }

    template <typename Pred>
    auto _enumerable_all_impl(Pred pred) const {
      for (auto &x : *this)
        if (!pred(x)) return false;
      return true;
    }

    template <typename Pred>
    auto _enumerable_any_impl(Pred pred) const {
      for (auto &x : *this)
        if (pred(x)) return true;
      return false;
    }

    template <typename Iterator>
    auto _enumerable_iterator_impl(Iterator iterator) const {
      for (auto &x : *this) iterator(x);
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

    auto _enumerable_head_impl() const {
      return &this->front();
    }

    auto _enumerable_append_impl(const T &e) {
      return this->push_back(e);
    }

    auto _enumerable_add_impl(const T &e) const {
      list<T> second = *this;
      second.push_back(e);
      return second;
    }

    auto _enumerable_at(uint64_t index) {
      return (*this)[index];
    }

    auto _enumerable_at_const(uint64_t index) const {
      return (*this)[index];
    }

    auto _enumerable_contains_impl(const T &e) const {
      return this->contains(e);
    }
    
    auto _enumerable_indexof_impl(const T &e) const {
      for (uint64_t i = 0; i < length(); i++) {
        if ((*this)[i] == e) return i;
      }

      return _enumerable_end_impl();
    }

    auto _enumerable_end_impl() const {
      return length() + 1;
    }

    inline uint64_t length() const override { return this->size(); }

    list<T> join(const list<list<T>> lists) const {
      list<T> copy = (*this);
      for (const auto &listel : lists) {
        for (const auto &element : listel) {
          copy.append(element);
        }
      }

      return copy;
    }

    template <typename NewType>
    list<NewType> convert() const {
      list<NewType> vec = {};
      for (const auto &x : (*this)) vec.push_back(NewType(x));
      return vec;
    }

    template <typename NewType, typename Cast>
    list<NewType> convert() const {
      list<NewType> vec = {};
      for (const auto &x : (*this)) vec.push_back((Cast)x);
      return vec;
    }

    template <typename NewType, auto CastFunction>
    list<NewType> convert() const {
      list<NewType> vec = {};
      for (const auto &x : (*this)) vec.push_back(CastFunction(x));
      return vec;
    }
  };
}