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

#ifndef YUM_INCLUDE_GUARD_VARDEF_H
#define YUM_INCLUDE_GUARD_VARDEF_H

#include "types.h"

/**
 * @brief variant type, holding a value which can change between several types.
 * Used by the engine in order to communicate between the engine and Lua.
 */
struct variant_struct {
  union {
    integer_t integer;
    number_t  number;
    boolean_t boolean;
    nil_t     nil;
    vuid_t    uid;
    lstring_t lstring;
    binary_t  binary;
  } hold;
  
  enum {
    VARIANT_NIL,
    VARIANT_INTEGER,
    VARIANT_NUMBER,
    VARIANT_BOOL,
    VARIANT_STRING,
    VARIANT_BINARY,
    VARIANT_UID,
  } type;
};

typedef struct variant_struct variant_t;

#endif // YUM_INCLUDE_GUARD_VARDEF_H