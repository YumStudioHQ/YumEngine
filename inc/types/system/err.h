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

#ifndef YUM_INCLUDE_GUARD_SYSERR_H
#define YUM_INCLUDE_GUARD_SYSERR_H

#include "types/base/types.h"
#include "utils/ystringutils.h"
#include "_byumlibc.h"

/**
 * @brief Returned by some functions, represents an error-state.
 */
typedef struct {
  enum {
    OK,
    ERROR,
    UNKNOWN_ERROR,
    INVALID_POINTER,
    FILE_NOT_FOUND,
    NOT_A_TABLE,
    INVALID_TYPE,
    NULL_OR_EMPTY_ARGUMENT,
    LUA_EXECUTION_ERROR,
    ILL_FUNCTION_PATH,
    PROMOTED_CXX_EXCEPTION,
    SDK_EXCEPTION,
  } category;

  struct {
    lstring_t func;
    lstring_t file;
    integer_t line;
  } source;
  lstring_t comment;
} syserr_t;

#define YUM_IMTOSTRING(a, x) #a#x

#define yummakeerror_x(what,cat,...) syserr_t {                        \
  .category = cat,                                                   \
  .source = { .func = lstring_from_string(__func__), .file = lstring_from_string(__FILE__), .line = __LINE__}, \
  .comment = lstring_from_string(YUM_IMTOSTRING(what, __VA_ARGS__)) }

#define yummakeerror(what,cat,...) yummakeerror_x(what, cat, __VA_ARGS__);
#define yummakeerror_runtime(what,cat) syserr_t {                        \
  .category = cat,                                                   \
  .source = { .func = lstring_from_string(__func__), .file = lstring_from_string(__FILE__), .line = __LINE__}, \
  .comment = lstring_from_string(what) };

#define yumsuccess yummakeerror_x("success!", syserr_t::OK)

/** 
 * @brief Formats an error. The given lstring owns the string.
 * @return Formated syserr_t in a lstring_t.
 */
yumlibc_cfun lstring_t yumfmterr(syserr_t);

/**
 * @brief Returns an ASCII value of the syserr_t's error category.
 * @return A const char* string, you may not free it. (Literal string)
 */
yumlibc_cfun const char *category_to_ascii(syserr_t);

/**
 * @brief Prints a formated error to STDOUT.
 */
yumlibc_cfun void yumprinterr(syserr_t);

#endif // !YUM_INCLUDE_GUARD_SYSERR_H