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

#include "inc/types/base/types.h"
#include "inc/types/base/vardef.h"

#include "inc/_byumlibc.h"

#include <string>

/**
 * @warning Not a part of the C API ! This implementation provides a viewable string until the next call on the same thread.
 */
extern "C" {
  utf8 yumlibc_dllattribute yumlibc_library_member(variant2strview)(const variant_t var) {
    thread_local std::string mbuff;
    mbuff = "<nil>";

    switch (var.type) {
      case variant_t::VARIANT_INTEGER:
        mbuff = std::to_string(var.hold.integer);
        break;
      case variant_t::VARIANT_NUMBER:
        mbuff = std::to_string(var.hold.number);
        break;
      case variant_t::VARIANT_BOOL:
        mbuff = var.hold.boolean ? "true" : "false";
        break;
      case variant_t::VARIANT_UID:
        mbuff = std::to_string(var.hold.uid.bytes);
        break;
      case variant_t::VARIANT_STRING:
        mbuff = std::string(var.hold.lstring.start, var.hold.lstring.length);
        break;
      case variant_t::VARIANT_BINARY:
        mbuff = std::string((char*)var.hold.binary.start, var.hold.binary.length);
        break;
      default: break;
    }

    return mbuff.c_str();
  }
}