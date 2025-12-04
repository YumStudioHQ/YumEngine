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

#include "base/types.h"
#include "base/vardef.h"
#include "yumem.hpp"

namespace YumEngine::xV1 {
  /**
   * @brief Extends the variant_t C type, providing easier variant management.
   */
  class Variant {
  private:
    inline static bool lstring_are_equal(const lstring_t &a, const lstring_t &b) {
      if (a.length != b.length) return false;
      for (integer_t i = 0; i < a.length; i++) {
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
      switch (raw.type) {
        case raw.VARIANT_STRING:
          if (raw.hold.lstring.owns) yumfree(raw.hold.lstring.start);
          break;
        case raw.VARIANT_BINARY:
          if (raw.hold.binary.owns) yumfree(raw.hold.binary.start);
          break;
        default:
          break;
      }
    }

    variant_t raw;

  public:
    inline Variant() {
      raw.hold.binary = {};
      raw.type = raw.VARIANT_NIL;
    }

    inline Variant(const variant_t &var) { raw = var; }

    inline Variant(const integer_t &in) {
      raw.hold.integer = in;
      raw.type = raw.VARIANT_INTEGER;
    }

    inline Variant(const number_t &in) {
      raw.hold.number = in;
      raw.type = raw.VARIANT_NUMBER;
    }

    inline Variant(const boolean_t &in) {
      raw.hold.boolean = in;
      raw.type = raw.VARIANT_BOOL;
    }

    inline Variant(const vuid_t &in) {
      raw.hold.uid = in;
      raw.type = raw.VARIANT_UID;
    }

    inline Variant(const lstring_t &in) {
      raw.hold.lstring = in;
      raw.type = raw.VARIANT_STRING;
    }

    inline Variant(const binary_t &in) {
      raw.hold.binary = in;
      raw.type = raw.VARIANT_BINARY;
    }

    Variant &operator=(const integer_t &in) {
      on_type_changes();
      raw.hold.integer = in;
      raw.type = raw.VARIANT_INTEGER;
      return (*this);
    }

    Variant &operator=(const number_t &in) {
      on_type_changes();
      raw.hold.number = in;
      raw.type = raw.VARIANT_NUMBER;
      return (*this);
    }

    Variant &operator=(const lstring_t &in) {
      on_type_changes();
      raw.hold.lstring = in;
      raw.type = raw.VARIANT_STRING;
      return (*this);
    }

    Variant &operator=(const binary_t &in) {
      on_type_changes();
      raw.hold.binary = in;
      raw.type = raw.VARIANT_BINARY;
      return (*this);
    }

    Variant &operator=(const vuid_t &in) {
      on_type_changes();
      raw.hold.uid = in;
      raw.type = raw.VARIANT_UID;
      return (*this);
    }

    Variant &operator=(const boolean_t &in) {
      on_type_changes();
      raw.hold.boolean = in;
      raw.type = raw.VARIANT_BOOL;
      return (*this);
    }

    bool operator==(const Variant &left) const {
      if (left.raw.type != raw.type) return false;
      switch (raw.type) {
        case raw.VARIANT_INTEGER: 
          return raw.hold.integer == left.raw.hold.integer;
        case raw.VARIANT_NUMBER: 
          return raw.hold.number == left.raw.hold.number;
        case raw.VARIANT_BOOL:
          return raw.hold.boolean == left.raw.hold.boolean;
        case raw.VARIANT_NIL: return true; // What the hell ya compare NIL to NIL bruh
        case raw.VARIANT_STRING:
          return lstring_are_equal(raw.hold.lstring, left.raw.hold.lstring);
        case raw.VARIANT_BINARY:
          return binaries_are_equal(raw.hold.binary, left.raw.hold.binary);
        case raw.VARIANT_UID:
          return raw.hold.uid.bytes == left.raw.hold.uid.bytes;
        default: return false;
      }
    }

    inline void reset() {
      on_type_changes();
      raw.hold.nil = {};
      raw.type = raw.VARIANT_NIL;
    }

    inline integer_t as_integer() const {
      return raw.hold.integer;
    }

    inline number_t as_number() const {
      return raw.hold.number;
    }

    inline boolean_t as_boolean() const {
      return raw.hold.boolean;
    }

    inline lstring_t as_string() const {
      return raw.hold.lstring;
    }

    inline binary_t as_binary() const {
      return raw.hold.binary;
    }

    inline vuid_t as_uid() const {
      return raw.hold.uid;
    }

    inline variant_t &c() { return raw; }
    inline const variant_t &c() const { return raw; }

    operator variant_t() { return raw; }
  };
}
