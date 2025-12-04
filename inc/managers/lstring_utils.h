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
 *                            https://github.com/YumStudioHQ                         *
 *                              https://github.com/wys-prog                          *
 *                                                                                   *
 *************************************************************************************/

#ifndef YUM_INCLUDE_GUARD_LSTRING_MANA_UTILS_H
#define YUM_INCLUDE_GUARD_LSTRING_MANA_UTILS_H

#include "types/base/types.h"
#include "_byumlibc.h"

yumlibc_cfun lstring_t alloc_lstring(integer_t);
yumlibc_cfun void      free_lstring(lstring_t);

#endif // YUM_INCLUDE_GUARD_LSTRING_MANA_UTILS_H