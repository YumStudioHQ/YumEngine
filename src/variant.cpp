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

#include "inc/yumtable.hpp"
#include "inc/pinlist.hpp"
#include "inc/variant.hpp"
#include "inc/yumsys.hpp"
#include "inc/yumdec.h"
#include "inc/glob.hpp"
#include "inc/exceptions.hpp"

#include <format>
#include <string>
#include <memory>

namespace YumEngine {

  Variant::Variant()                       : value_(std::monostate{}), kind_(NIL) { }
  Variant::Variant(int64_t i)              : value_(i), kind_(INTEGER) {}
  Variant::Variant(double d)               : value_(d), kind_(NUMBER) {}
  Variant::Variant(const std::string &s)   : value_(s), kind_(STRING) {}
  Variant::Variant(bool b)                 : value_(b), kind_(BOOLEAN) {}
  Variant::Variant(const YumBinaryBlob &b) : value_(b), kind_(BINARY) {}
  Variant::Variant(const Table &table)     : value_(table), kind_(TABLE) {}
  Variant::Variant(const YumUID &uid)      : value_(uid), kind_(UID) {}

  // Copy & Move
  Variant::Variant(const Variant &) = default;
  Variant::Variant(Variant &&) noexcept = default;
  Variant &Variant::operator=(const Variant &) = default;
  Variant &Variant::operator=(Variant &&) noexcept = default;

  // Setters
  void Variant::set(int64_t i)              { value_ = i; kind_ = INTEGER; }
  void Variant::set(double d)               { value_ = d; kind_ = NUMBER; }
  void Variant::set(const std::string &s)   { value_ = s; kind_ = STRING; }
  void Variant::set(bool b)                 { value_ = b; kind_ = BOOLEAN; }
  void Variant::set(const YumBinaryBlob &b) { value_ = b; kind_ = BINARY; }
  void Variant::set(const Table &table)     { value_ = table; kind_ = TABLE; }
  void Variant::set(const YumUID &uid)      { value_ = uid; kind_ = UID; }
  void Variant::clear()                     { value_ = std::monostate{}; kind_ = NIL; }

  // Getters
  int64_t Variant::as_int() const           { return std::get<int64_t>(value_); }
  double Variant::as_float() const          { return std::get<double>(value_); }
  const std::string &Variant::as_string() const { return std::get<std::string>(value_); }
  bool Variant::as_bool() const             { return std::get<bool>(value_); }
  YumBinaryBlob Variant::as_binary() const  { return std::get<YumBinaryBlob>(value_); }
  Variant::Table Variant::as_table() const  { return std::get<Table>(value_); }
  YumUID Variant::as_uid() const            { return std::get<YumUID>(value_); }
  bool Variant::has_value() const           { return !std::holds_alternative<std::monostate>(value_); }

  // Type checks
  bool Variant::is_int() const              { return std::holds_alternative<int64_t>(value_); }
  bool Variant::is_float() const            { return std::holds_alternative<double>(value_); }
  bool Variant::is_string() const           { return std::holds_alternative<std::string>(value_); }
  bool Variant::is_bool() const             { return std::holds_alternative<bool>(value_); }
  bool Variant::is_binary() const           { return std::holds_alternative<YumBinaryBlob>(value_); }
  bool Variant::is_table() const            { return std::holds_alternative<std::shared_ptr<YumTable>>(value_); }
  bool Variant::is_uid() const              { return std::holds_alternative<YumUID>(value_); }
}

extern "C" {

YUM_OUTATR YumVariant *YumVariant_new(void) {
  auto var = new YumVariant();
  YumEngine::G_yglob().get().pin(var);

  return var;
}

YUM_OUTATR void YumVariant_delete(YumVariant *var) {
  var->free();
  if (var->get_pin()) var->get_pin()->object_freed = true;
  delete var;
}

// Setters
YUM_OUTATR void YumVariant_setInt(YumVariant *var, int64_t v) {
  if (var) var->set(v);
}

YUM_OUTATR void YumVariant_setFloat(YumVariant *var, double v) {
  if (var) var->set(v);
}

YUM_OUTATR void YumVariant_setBool(YumVariant *var, int32_t v) {
  if (var) var->set(v != 0);
}

YUM_OUTATR void YumVariant_setString(YumVariant *var, const char *str) {
  if (var && str) var->set(std::string(str));
}

YUM_OUTATR void YumVariant_setRawBytes(YumVariant *var, YumBinaryBlob blob) {
  if (var) var->set(blob);
}

YUM_OUTATR void YumVariant_setUid(YumVariant *var, YumUID uid) {
  if (var) var->set(uid);
}

// Getters
YUM_OUTATR int64_t YumVariant_asInt(const YumVariant *var) {
  try {
    return var ? var->as_int() : 0;
  } catch (const std::exception &e) {
    (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
  } catch (...) {
    (*G_err()) << std::format("yum: G_sys: err: unknown exception caught") << std::endl;
  }
  YUM_AFTER_THROW();
  return 0;
}

YUM_OUTATR double YumVariant_asFloat(const YumVariant *var) {
  try {
    return var ? var->as_float() : 0.0;
  } catch (const std::exception &e) {
    (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
  } catch (...) {
    (*G_err()) << std::format("yum: G_sys: err: unknown exception caught") << std::endl;
  }
  YUM_AFTER_THROW();
  return 0.0;
}

YUM_OUTATR int32_t YumVariant_asBool(const YumVariant *var) {
  try {
    return var ? static_cast<int32_t>(var->as_bool()) : 0;
  } catch (const std::exception &e) {
    (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
  } catch (...) {
    (*G_err()) << std::format("yum: G_sys: err: unknown exception caught") << std::endl;
  }
  YUM_AFTER_THROW();
  return 0;
}

YUM_OUTATR const char *YumVariant_asString(const YumVariant *var) {
  try {
    thread_local std::string temp;
    if (var) temp = var->as_string();
    else temp.clear();
    return temp.c_str();
  } catch (const std::exception &e) {
    (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
  } catch (...) {
    (*G_err()) << std::format("yum: G_sys: err: unknown exception caught") << std::endl;
  }
  YUM_AFTER_THROW();
  return "";
}

YUM_OUTATR YumBinaryBlob YumVariant_asBinary(const YumVariant *var) {
  try {
    return var ? var->as_binary() : YumBinaryBlob{.start = nullptr, .size = 0};
  } catch (const std::exception &e) {
    (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
  } catch (...) {
    (*G_err()) << std::format("yum: G_sys: err: unknown exception caught") << std::endl;
  }
  YUM_AFTER_THROW();
  return YumBinaryBlob {.start = nullptr, .size = 0};
}

YUM_OUTATR YumUID YumVariant_asUID(const YumVariant *var) {
    try {
    return var ? var->as_uid() : YumUID{.bytes=0};
  } catch (const std::exception &e) {
    (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
  } catch (...) {
    (*G_err()) << std::format("yum: G_sys: err: unknown exception caught") << std::endl;
  }
  YUM_AFTER_THROW();
  return YumUID{.bytes=0};
}

YUM_OUTATR YumCTable *YumVariant_asTable(const YumVariant *var) {
  try {
    return var ? var->as_table().get() : nullptr;
  } catch (const std::exception &e) {
    (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
  } catch (...) {
    (*G_err()) << std::format("yum: G_sys: err: unknown exception caught") << std::endl;
  }
  YUM_AFTER_THROW();
  return nullptr;
}

// Type checks
YUM_OUTATR int32_t YumVariant_isInt(const YumVariant *var) {
  return var ? static_cast<int32_t>(var->is_int()) : 0;
}

YUM_OUTATR int32_t YumVariant_isFloat(const YumVariant *var) {
  return var ? static_cast<int32_t>(var->is_float()) : 0;
}

YUM_OUTATR int32_t YumVariant_isBool(const YumVariant *var) {
  return var ? static_cast<int32_t>(var->is_bool()) : 0;
}

YUM_OUTATR int32_t YumVariant_isString(const YumVariant *var) {
  return var ? static_cast<int32_t>(var->is_string()) : 0;
}

YUM_OUTATR int32_t YumVariant_isBinary(const YumVariant *var) {
  return var ? static_cast<int32_t>(var->is_binary()) : 0;
}

YUM_OUTATR int32_t YumVariant_isTable(const YumVariant *var) {
  return var ? static_cast<int32_t>(var->is_table()) : 0;
}

YUM_OUTATR int32_t YumVariant_isUid(const YumVariant *var) {
  return var ? static_cast<int32_t>(var->is_uid()) : 0;
}

} // extern "C"
