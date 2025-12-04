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

#ifndef YUM_INCLUDE_GUARD_YSTRING_UTILS_H
#define YUM_INCLUDE_GUARD_YSTRING_UTILS_H

#include "types/base/types.h"
#include "_byumlibc.h"

/** 
 * @brief Creates a lstring from a C-like string (Must be null-terminated). 
 * @note The lstring won't hold the string, so if you delete the given string, your lstring will point
 * invalid memory address.
 */
yumlibc_cfun lstring_t lstring_from_string(const char*);

/** @brief Returns a C-like string from the given lstring. You must free it. */
yumlibc_cfun const char *lstring_to_string(const lstring_t);

/** 
 * @brief Creates a lstring from a C-like string (Must be null-terminated).
 * @note The returned lstring will hold the string. You may free it.
 */
yumlibc_cfun lstring_t surelstring_from_string(const char*);

#endif // YUM_INCLUDE_GUARD_YSTRING_UTILS_H