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

#ifndef YUM_INCLUDE_GUARD_TYPES_H
#define YUM_INCLUDE_GUARD_TYPES_H

/**
 * @file types.h
 * @brief Defines global types to use in C, C++, and also other languages, as it uses fixed size types.
 * Know that the API assumes that double is always 64 bits. This will change to std::float64_t once the
 * C++ standard 23 will be widely supported.
 */

#include <stdint.h>

typedef int64_t     integer_t;
typedef double      number_t;
typedef int8_t      boolean_t;
typedef struct {}   nil_t;

typedef struct {
  uint64_t bytes;
} vuid_t;

typedef struct {
  const char *start;
  uint64_t    length;
  boolean_t   owns;
} lstring_t;

typedef const char *utf8;

typedef struct {
  const uint8_t *start;
  uint64_t       length;
  boolean_t      owns;
} binary_t;

#endif // YUM_INCLUDE_GUARD_TYPES_H