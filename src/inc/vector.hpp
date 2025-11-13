/*********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *            This file is free & open source            *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                          from                         *
 *                         MONOE                         *
 *                                                       *
 *********************************************************/
#pragma once

/**
 * @file vector.hpp
 * @brief Lightweight container for YumEngine Variant values.
 *
 * This header defines `YumEngine::Vector`, a minimal wrapper around `std::vector<Variant>`.
 * It is designed to bridge C++ and C APIs in the YumEngine runtime.
 * 
 * Only a subset of essential methods is provided to simplify language bindings and 
 * to maintain ABI stability across native and scripting layers.
 */

#include <vector>
#include <functional>
#include "variant.hpp"
#include "yumobject.hpp"

namespace YumEngine {

  /**
   * @class Vector
   * @brief Minimal, type-safe container for `Variant` values.
   *
   * `YumEngine::Vector` serves as a simplified wrapper over `std::vector<Variant>`.
   * It is primarily used for passing arguments and return values between
   * Lua scripts, native C++ code, and the C API.
   *
   * The class is intentionally limited in scope to make cross-language
   * interop (especially with C) simpler and predictable.
   *
   * Example usage:
   * @code
   * YumEngine::Vector args;
   * args.append(YumEngine::Variant(42));
   * args.append(YumEngine::Variant("hello"));
   *
   * for (const auto& v : args)
   *     std::cout << v.as_string() << std::endl;
   * @endcode
   */
  class Vector : public YumObject {
  private:
    /** @brief Internal storage for Variant values. */
    std::vector<Variant> internal;

  protected:
    inline void _free() const override {
      for (const auto &V : internal) { V.free(); }
    }

  public:
    /** @name Constructors and Destructor */
    /** @{ */

    /** @brief Constructs an empty Vector. */
    Vector();

    /** @brief Destroys the Vector and releases resources. */
    ~Vector();

    /** @} */

    /** @name Element Manipulation */
    /** @{ */

    /**
     * @brief Appends a new Variant to the container.
     * @param value Variant value to append.
     */
    void append(const Variant &value);

    /**
     * @brief Removes the last element from the container.
     * 
     * If the container is empty, this operation has no effect.
     */
    void pop();

    /**
     * @brief Clears all elements from the container.
     */
    void clear();

    /** @} */

    /** @name Memory management */
    /** @{ */

    /**
     * @brief Reserves n bytes.
     * @param n count of bytes to reserve.
     */
    void reserve(uint64_t n);

    /**
     * @brief Reserves n bytes before an allocation.
     * @param n count of elements that will be added to the vector.
     */
    void preappend(uint64_t n);

    /** @} */

    /** @name Element Access */
    /** @{ */

    /**
     * @brief Returns the number of elements in the container.
     * @return Number of stored Variant elements.
     */
    int64_t size() const;

    /**
     * @brief Accesses an element at the specified index.
     * @param index Position of the element (0-based).
     * @return Reference to the Variant at the given index.
     * @warning No bounds checking is performed.
     */
    Variant &at(int64_t index);

    /**
     * @brief Accesses an element at the specified index (const version).
     * @param index Position of the element (0-based).
     * @return Const reference to the Variant at the given index.
     * @warning No bounds checking is performed.
     */
    const Variant &at(int64_t index) const;

    /** @} */

    /** @name Iteration Helpers (C++ Only) */
    /** @{ */

    /**
     * @brief Iterates over all elements (read-only).
     * @param func Function called for each element.
     *
     * Example:
     * @code
     * vec.foreach([](const Variant& v) {
     *     std::cout << v.as_int() << std::endl;
     * });
     * @endcode
     */
    void foreach(const std::function<void(const Variant&)> &func) const;

    /**
     * @brief Iterates over all elements (mutable access).
     * @param func Function called for each element.
     *
     * Example:
     * @code
     * vec.foreach([](Variant& v) {
     *     if (v.is_int()) v.set(v.as_int() * 2);
     * });
     * @endcode
     */
    void foreach(const std::function<void(Variant&)> &func);

    /** @} */
  };

} // namespace YumEngine

/**
 * @typedef YumVector
 * @brief Alias for `YumEngine::Vector` (used by the C API).
 */
using YumVector = YumEngine::Vector;
