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
 * @file table.hpp
 * @brief C++ Table container bridging Lua and C APIs.
 *
 * Defines the `YumEngine::YumTable` class, a lightweight associative container
 * mapping `Variant` keys to `Variant` values.  
 * This structure is the internal representation used by the C API’s `YumCTable`.
 *
 * @warning Any table allocated and exposed through the C API (`YumCTable_new()`)
 *          must be explicitly freed with `YumCTable_delete()`.
 */

#include "yumdec.h"
#include "variant.hpp"
#include "vector.hpp"

#include <string>
#include <cstdint>
#include <cstring>
#include <unordered_map>

namespace std {
  template <>
  struct hash<YumEngine::Variant> {
    inline size_t operator()(const YumEngine::Variant &v) const noexcept {
      using Kind = YumEngine::Variant::VariantKind;
      size_t h = static_cast<size_t>(v.get_kind());
      auto mix = [](size_t x) {
        uint64_t y = static_cast<uint64_t>(x);
        y ^= (y >> 23);
        y *= 0x2127599bf4325c37ULL;
        y ^= (y >> 47);
        return static_cast<size_t>(y);
      };
      switch (v.get_kind()) {
        case Kind::INTEGER: return mix(h ^ std::hash<long long>()(v.as_int()));
        case Kind::NUMBER:  return mix(h ^ std::hash<double>()(v.as_float()));
        case Kind::BOOLEAN: return mix(h ^ std::hash<bool>()(v.as_bool()));
        case Kind::STRING:  return mix(h ^ std::hash<std::string>()(v.as_string()));
        case Kind::NIL:     return mix(h);
        case Kind::TABLE:   return mix(h ^ std::hash<void*>()(v.as_table().get()));
        case Kind::BINARY: {
          auto bin = v.as_binary();
          return mix(h ^ std::hash<size_t>()(bin.size) ^
                     (reinterpret_cast<size_t>(bin.start) >> 4));
        }
        default: return h;
      }
    }
  };
}

namespace YumEngine {

  /**
   * @class YumTable
   * @brief Simple key-value container for `Variant` objects.
   *
   * `YumTable` provides a lightweight associative mapping between `Variant` keys and values.
   * It is designed to resemble Lua’s table behavior while maintaining C++ interoperability
   * with YumEngine’s scripting and C APIs.
   *
   * The container supports checking for key existence, retrieving keys/values,
   * and accessing stored values directly by key.
   *
   * Example:
   * @code
   * YumEngine::YumTable tbl;
   * tbl.at(Variant("score")) = Variant(100);
   *
   * if (tbl.has_key(Variant("score")))
   *     std::cout << tbl.at(Variant("score")).as_int() << std::endl;
   * @endcode
   */
  class YumTable {
  private:
    /** @brief Internal associative storage (maps Variant → Variant). */
    std::unordered_map<Variant, Variant> map;

  public:
    /** @name Lookup and Access */
    /** @{ */

    /**
     * @brief Checks whether a given key exists in the table.
     * @param var Variant key to check.
     * @return `true` if the key exists, otherwise `false`.
     */
    inline bool has_key(const Variant &var) const {
      return map.contains(var);
    }

    /**
     * @brief Returns a list of all keys stored in the table.
     * @return Vector containing all keys as Variant objects.
     */
    inline Vector keys() const {
      Vector vec = {};
      for (const auto &[K, _] : map)
        vec.append(K);
      return vec;
    }

    /**
     * @brief Returns a list of all values stored in the table.
     * @return Vector containing all values as Variant objects.
     */
    inline Vector values() const {
      Vector vec = {};
      for (const auto &[_, V] : map)
        vec.append(V);
      return vec;
    }

    /**
     * @brief Returns a value associated with a given key.
     * @param key Variant representing the key to access.
     * @return The corresponding value.
     * @throws std::out_of_range if the key does not exist.
     */
    inline Variant at(const Variant &key) const {
      return map.at(key);
    }

    /**
     * @brief Sets @key to @value
     * @param key The key value
     * @param value The value
     */
    inline void set(const Variant &key, const Variant &value) {
      map[key] = value;
    }

    /**
     * @brief iterates all elements of the table by giving to the given callback the key and its value.
     */
    inline void foreach(const std::function<void(const Variant&, const Variant&)> &fun) const {
      for(const auto &[K, V] : map) {
        fun(K, V);
      }
    }

    /** @} */
  };

} // namespace YumEngine

/**
 * @typedef YumCTable
 * @brief Alias for `YumEngine::YumTable` (used by the C API).
 *
 * @warning Objects of this type allocated via the C API must be released 
 *          with `YumCTable_delete()` to avoid memory leaks.
 */
using YumCTable = YumEngine::YumTable;
