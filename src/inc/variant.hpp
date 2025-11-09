#pragma once

#include <cstdint>
#include <string>
#include <variant>

#include "yumdec.h"

namespace Yumcxx {
  class Variant {
  private:
    using ValueType = std::variant<std::monostate, int64_t, 
      double, std::string, bool, YumBinaryBlob>;
    ValueType value_;

  public:
    Variant();
    Variant(int64_t i);
    Variant(double d);
    Variant(const std::string &s);
    Variant(bool b);
    Variant(const YumBinaryBlob &b);

    // Copy & Move
    Variant(const Variant &);
    Variant(Variant &&) noexcept;
    Variant &operator=(const Variant &);
    Variant &operator=(Variant &&) noexcept;

    // Setters
    void set(int64_t i);
    void set(double d);
    void set(const std::string &s);
    void set(bool b);
    void set(const YumBinaryBlob &blob);
    void clear();

    // Getters
    int64_t as_int() const;
    double as_float() const;
    const std::string &as_string() const;
    bool as_bool() const;
    YumBinaryBlob as_binary() const;
    bool has_value() const;

    // Type checks
    bool is_int() const;
    bool is_float() const;
    bool is_string() const;
    bool is_bool() const;
    bool is_binary() const;
  };

} // namespace Yumcxx

using YumVariant = Yumcxx::Variant;
