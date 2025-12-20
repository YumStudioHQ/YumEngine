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

#include <cstring>
#include <cstdlib>
#include <string>

#include "base/types.h"
#include "base/vardef.h"
#include "inc/yumem.hpp"

namespace YumEngine::xV1 {
  /**
   * @brief Extends the variant_t C type, providing easier variant management.
   */
  class CVariant {
  private:
    variant_t raw;

    inline static bool lstring_are_equal(const lstring_t &a, const lstring_t &b) {
      if (a.length != b.length) return false;
      for (uint64_t i = 0; i < a.length; i++) {
        if (a.start[i] != b.start[i]) return false;
      }
      return true;
    }

    inline static bool binaries_are_equal(const binary_t &a, const binary_t &b) {
      return lstring_are_equal(
        lstring_t{.start = (const char*)a.start, .length = a.length, .owns = false},
        lstring_t{.start = (const char*)b.start, .length = b.length, .owns = false}
      ); /* This should be illegal fr */
    }

    inline void on_type_changes() {
      switch (this->raw.type) {
        case variant_t::VARIANT_STRING:
          if (this->raw.hold.lstring.owns) yumfree(this->raw.hold.lstring.start);
          break;
        case variant_t::VARIANT_BINARY:
          if (this->raw.hold.binary.owns) yumfree(this->raw.hold.binary.start);
          break;
        default:
          break;
      }
    }

  public:
    inline CVariant() {
      this->raw.hold.binary = {};
      this->raw.type = this->raw.VARIANT_NIL;
    }

    inline CVariant(const variant_t &var) { raw = var; }

    inline CVariant(const integer_t &in) {
      this->raw.hold.integer = in;
      this->raw.type = this->raw.VARIANT_INTEGER;
    }

    inline CVariant(const number_t &in) {
      this->raw.hold.number = in;
      this->raw.type = this->raw.VARIANT_NUMBER;
    }

    inline CVariant(const boolean_t &in) {
      this->raw.hold.boolean = in;
      this->raw.type = this->raw.VARIANT_BOOL;
    }

    inline CVariant(const vuid_t &in) {
      this->raw.hold.uid = in;
      this->raw.type = this->raw.VARIANT_UID;
    }

    inline CVariant(const lstring_t &in) {
      this->raw.hold.lstring = in;
      this->raw.type = this->raw.VARIANT_STRING;
    }

    inline CVariant(const binary_t &in) {
      this->raw.hold.binary = in;
      this->raw.type = this->raw.VARIANT_BINARY;
    }

    CVariant &operator=(const integer_t &in) {
      on_type_changes();
      this->raw.hold.integer = in;
      this->raw.type = this->raw.VARIANT_INTEGER;
      return (*this);
    }

    CVariant &operator=(const number_t &in) {
      on_type_changes();
      this->raw.hold.number = in;
      this->raw.type = this->raw.VARIANT_NUMBER;
      return (*this);
    }

    CVariant &operator=(const lstring_t &in) {
      on_type_changes();
      this->raw.hold.lstring = in;
      this->raw.type = this->raw.VARIANT_STRING;
      return (*this);
    }

    CVariant &operator=(const binary_t &in) {
      on_type_changes();
      this->raw.hold.binary = in;
      this->raw.type = this->raw.VARIANT_BINARY;
      return (*this);
    }

    CVariant &operator=(const vuid_t &in) {
      on_type_changes();
      this->raw.hold.uid = in;
      this->raw.type = this->raw.VARIANT_UID;
      return (*this);
    }

    CVariant &operator=(const boolean_t &in) {
      on_type_changes();
      this->raw.hold.boolean = in;
      this->raw.type = this->raw.VARIANT_BOOL;
      return (*this);
    }

    bool operator==(const CVariant &left) const {
      if (left.raw.type != this->raw.type) return false;
      switch (this->raw.type) {
        case variant_t::VARIANT_INTEGER: 
          return raw.hold.integer == left.raw.hold.integer;
        case variant_t::VARIANT_NUMBER: 
          return raw.hold.number == left.raw.hold.number;
        case variant_t::VARIANT_BOOL:
          return raw.hold.boolean == left.raw.hold.boolean;
        case variant_t::VARIANT_NIL: return true; // What the hell ya compare NIL to NIL bruh
        case variant_t::VARIANT_STRING:
          return lstring_are_equal(raw.hold.lstring, left.raw.hold.lstring);
        case variant_t::VARIANT_BINARY:
          return binaries_are_equal(raw.hold.binary, left.raw.hold.binary);
        case variant_t::VARIANT_UID:
          return raw.hold.uid.bytes == left.raw.hold.uid.bytes;
        default: return false;
      }
    }

    inline void reset() {
      on_type_changes();
      this->raw.hold.nil = {};
      this->raw.type = this->raw.VARIANT_NIL;
    }

    inline integer_t as_integer() const {
      return this->raw.hold.integer;
    }

    inline number_t as_number() const {
      return this->raw.hold.number;
    }

    inline boolean_t as_boolean() const {
      return this->raw.hold.boolean;
    }

    inline lstring_t as_string() const {
      return this->raw.hold.lstring;
    }

    inline binary_t as_binary() const {
      return this->raw.hold.binary;
    }

    inline vuid_t as_uid() const {
      return this->raw.hold.uid;
    }

    inline variant_t &c() { return this->raw; }
    inline const variant_t &c() const { return this->raw; }

    operator variant_t() { return this->raw; }

    inline std::string to_string() const {
      switch (raw.type) {
        case variant_t::VARIANT_INTEGER:
          return std::to_string(raw.hold.integer);
        case variant_t::VARIANT_NUMBER:
          return std::to_string(raw.hold.number);
        case variant_t::VARIANT_BOOL:
          return raw.hold.boolean ? "true" : "false";
        case variant_t::VARIANT_NIL:
          return "nil";
        case variant_t::VARIANT_STRING:
          return std::string(raw.hold.lstring.start, raw.hold.lstring.length);
        case variant_t::VARIANT_BINARY:
          return "<binary data>";
        case variant_t::VARIANT_UID: {
          return std::to_string(raw.hold.uid.bytes);
        }
      }

      return "<unknown>";
    }
  };
}
