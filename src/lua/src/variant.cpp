#include "inc/variant.hpp"
#include "inc/yumdec.h"

#include <string>

namespace Yumcxx {

    Variant::Variant() = default;
    Variant::Variant(int64_t i)             : value_(i) {}
    Variant::Variant(double d)              : value_(d) {}
    Variant::Variant(const std::string &s)  : value_(s) {}
    Variant::Variant(bool b)                : value_(b) {}

    // Copy & Move
    Variant::Variant(const Variant &) = default;
    Variant::Variant(Variant &&) noexcept = default;
    Variant &Variant::operator=(const Variant &) = default;
    Variant &Variant::operator=(Variant &&) noexcept = default;

    // Setters
    void Variant::set(int64_t i)             { value_ = i; }
    void Variant::set(double d)              { value_ = d; }
    void Variant::set(const std::string &s)  { value_ = s; }
    void Variant::set(bool b)                { value_ = b; }
    void Variant::clear()                    { value_ = std::monostate{}; }

    // Getters
    int64_t Variant::as_int() const           { return std::get<int64_t>(value_); }
    double Variant::as_float() const          { return std::get<double>(value_); }
    const std::string &Variant::as_string() const { return std::get<std::string>(value_); }
    bool Variant::as_bool() const             { return std::get<bool>(value_); }
    bool Variant::has_value() const           { return !std::holds_alternative<std::monostate>(value_); }

    // Type checks
    bool Variant::is_int() const              { return std::holds_alternative<int64_t>(value_); }
    bool Variant::is_float() const            { return std::holds_alternative<double>(value_); }
    bool Variant::is_string() const           { return std::holds_alternative<std::string>(value_); }
    bool Variant::is_bool() const             { return std::holds_alternative<bool>(value_); }

}

extern "C" {

YUM_OUTATR YumVariant *YumVariant_new(void) {
  return new YumVariant();
}

YUM_OUTATR void YumVariant_delete(YumVariant *var) {
  delete var;
}

// Setters
YUM_OUTATR void YumVariant_setInt(YumVariant *var, int64_t v) {
  if (var) var->set(v);
}

YUM_OUTATR void YumVariant_setFloat(YumVariant *var, double v) {
  if (var) var->set(v);
}

YUM_OUTATR void YumVariant_setBool(YumVariant *var, int v) {
  if (var) var->set(v != 0);
}

YUM_OUTATR void YumVariant_setString(YumVariant *var, const char *str) {
  if (var && str) var->set(std::string(str));
}

// Getters
YUM_OUTATR int64_t YumVariant_asInt(const YumVariant *var) {
  return var ? var->as_int() : 0;
}

YUM_OUTATR double YumVariant_asFloat(const YumVariant *var) {
  return var ? var->as_float() : 0.0;
}

YUM_OUTATR int YumVariant_asBool(const YumVariant *var) {
  return var ? static_cast<int>(var->as_bool()) : 0;
}

YUM_OUTATR const char *YumVariant_asString(const YumVariant *var) {
  static std::string temp;
  if (var) temp = var->as_string();
  else temp.clear();
  return temp.c_str();
}

// Type checks
YUM_OUTATR int YumVariant_isInt(const YumVariant *var) {
  return var ? static_cast<int>(var->is_int()) : 0;
}

YUM_OUTATR int YumVariant_isFloat(const YumVariant *var) {
  return var ? static_cast<int>(var->is_float()) : 0;
}

YUM_OUTATR int YumVariant_isBool(const YumVariant *var) {
  return var ? static_cast<int>(var->is_bool()) : 0;
}

YUM_OUTATR int YumVariant_isString(const YumVariant *var) {
  return var ? static_cast<int>(var->is_string()) : 0;
}

} // extern "C"
