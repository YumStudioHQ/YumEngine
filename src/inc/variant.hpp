/*********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *            This file is free & open source            *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                          from                         *
 *                         MONOE.                        *
 *                                                       *
 *********************************************************/

#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <cstring>
#include <variant>

#include "yumdec.h"
#include "yumobject.hpp"

namespace YumEngine {
  class YumTable;

/**
 * @class Variant
 * @brief Dynamically typed value container used for Lua <-> C++ communication.
 *
 * The `Variant` class represents any value type supported by the Yum Engine backend.
 * It wraps an internal `std::variant` that can hold:
 * - `int64_t` — integer values
 * - `double` — floating-point values
 * - `std::string` — UTF-8 text (don't use it for binary)
 * - `bool` — boolean values
 * - `YumBinaryBlob` — binary data (C-style structure)
 *
 * Variants are the primary mechanism used to exchange data between Lua and native code.
 *
 * @note For high-level bindings, prefer using language-specific wrappers or helpers
 *       rather than constructing `YumBinaryBlob` manually.
 */
class Variant : public YumObject {
public:
  enum VariantKind {
    NIL,
    INTEGER,
    NUMBER,
    STRING,
    BOOLEAN,
    BINARY,
    TABLE,
    UID,
  };

  using Table = std::shared_ptr<YumTable>;
  using ValueType = std::variant<std::monostate, int64_t, double, std::string, bool, YumBinaryBlob,
  std::shared_ptr<YumTable>, YumUID>;

private:
  ValueType value_;
  VariantKind kind_;

public:
  /** @name Constructors */
  /** @{ */

  /**
   * @brief Constructs an empty (nil) Variant.
   */
  Variant();

  /**
   * @brief Constructs a Variant containing an integer.
   * @param i Integer value.
   */
  Variant(int64_t i);

  /**
   * @brief Constructs a Variant containing a floating-point value.
   * @param d Floating-point value.
   */
  Variant(double d);

  /**
   * @brief Constructs a Variant containing a string.
   * @param s UTF-8 string or binary data.
   */
  Variant(const std::string &s);

  /**
   * @brief Constructs a Variant containing a boolean.
   * @param b Boolean value.
   */
  Variant(bool b);

  /**
   * @brief Constructs a Variant from a binary blob.
   * @param b Binary data blob.
   */
  Variant(const YumBinaryBlob &b);

  /**
   * @brief Constructs a Variant from a Lua table.
   * @param t Lua table.
   */
  Variant(const Table &t);

  /**
   * @brief Constructs a Variant from a UID.
   * @param uid A UID.
   */
  Variant(const YumUID &uid);

  /** @} */

  /** @name Copy and Move */
  /** @{ */

  /**
   * @brief Copy constructor.
   * @param other Variant to copy from.
   */
  Variant(const Variant &other);

  /**
   * @brief Move constructor.
   * @param other Variant to move from.
   */
  Variant(Variant &&other) noexcept;

  /**
   * @brief Copy assignment.
   * @param other Variant to copy from.
   * @return Reference to this Variant.
   */
  Variant &operator=(const Variant &other);

  /**
   * @brief Move assignment.
   * @param other Variant to move from.
   * @return Reference to this Variant.
   */
  Variant &operator=(Variant &&other) noexcept;

  /** @} */

  /** @name Setters */
  /** @{ */

  /**
   * @brief Sets the Variant to an integer value.
   * @param i Integer value.
   */
  void set(int64_t i);

  /**
   * @brief Sets the Variant to a floating-point value.
   * @param d Floating-point value.
   */
  void set(double d);

  /**
   * @brief Sets the Variant to a string value.
   * @param s UTF-8 string or binary data.
   */
  void set(const std::string &s);

  /**
   * @brief Sets the Variant to a boolean value.
   * @param b Boolean value.
   */
  void set(bool b);

  /**
   * @brief Sets the Variant to a binary blob.
   * @param blob Binary data blob.
   */
  void set(const YumBinaryBlob &blob);

  /**
   * @brief Sets the Variant to a lua table.
   * @param table Lua table
   */
  void set(const Table &table);

  /**
   * @brief Sets the Variant to a UID.
   * @param uid the UID
   */
  void set(const YumUID &uid);

  /**
   * @brief Clears the stored value, resetting to nil.
   */
  void clear();

  /** @} */

  /** @name Getters */
  /** @{ */

  /**
   * @brief Returns the Variant as an integer.
   * @return Integer value.
   * @throws std::bad_variant_access if type is not integer.
   */
  int64_t as_int() const;

  /**
   * @brief Returns the Variant as a floating-point number.
   * @return Floating-point value.
   * @throws std::bad_variant_access if type is not float.
   */
  double as_float() const;

  /**
   * @brief Returns the Variant as a string.
   * @return Reference to internal string.
   * @throws std::bad_variant_access if type is not string.
   */
  const std::string &as_string() const;

  /**
   * @brief Returns the Variant as a boolean.
   * @return Boolean value.
   * @throws std::bad_variant_access if type is not boolean.
   */
  bool as_bool() const;

  /**
   * @brief Returns the Variant as a binary blob.
   * @return Binary data blob.
   * @throws std::bad_variant_access if type is not binary.
   */
  YumBinaryBlob as_binary() const;

  /**
   * @brief Returns the Variant as a Lua table.
   * @return A table.
   * @throws std::bad_variant_access if type is not table.
   */
  Table as_table() const;

  /**
   * @brief Returns the Variant as a UID.
   * @return A UID.
   * @throws std::bad_variant_access if type is not uid.
   */
  YumUID as_uid() const;

  /**
   * @brief Checks if the Variant currently holds a non-nil value.
   * @return `true` if a value is stored, `false` if nil.
   */
  bool has_value() const;

  /** @} */

  /** @name Type Checks */
  /** @{ */

  /** @brief Returns true if the Variant contains an integer. */
  bool is_int() const;

  /** @brief Returns true if the Variant contains a floating-point value. */
  bool is_float() const;

  /** @brief Returns true if the Variant contains a string. */
  bool is_string() const;

  /** @brief Returns true if the Variant contains a boolean. */
  bool is_bool() const;

  /** @brief Returns true if the Variant contains binary data. */
  bool is_binary() const;

  /** @brief Returns true if the Variant a Lua table. */
  bool is_table() const;

  bool is_uid() const;
  /** @} */

  VariantKind get_kind() const { return kind_; }
};

inline bool operator==(const Variant &a, const Variant &b) {
    if (a.get_kind() != b.get_kind()) return false;

    switch (a.get_kind()) {
      case Variant::INTEGER: return a.as_int() == b.as_int();
      case Variant::NUMBER:  return a.as_float() == b.as_float();
      case Variant::BOOLEAN: return a.as_bool() == b.as_bool();
      case Variant::STRING:  return a.as_string() == b.as_string();
      case Variant::NIL:     return true;
      case Variant::TABLE:
        // Up to you: deep compare tables or just compare pointers.
        return a.as_table() == b.as_table();
      case Variant::BINARY: {
        auto ba = a.as_binary(), bb = b.as_binary();
        if (ba.size != bb.size) return false;
        return std::memcmp(ba.start, bb.start, ba.size) == 0;
      }
      case Variant::UID:    return a.as_uid().bytes == b.as_uid().bytes;
      default: return false;
    }
  }

} // namespace YumEngine

/**
 * @typedef YumVariant
 * @brief Alias for YumEngine::Variant (C-API compatible).
 */
using YumVariant = YumEngine::Variant;
