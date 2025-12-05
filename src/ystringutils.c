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
#include "inc/yummem.h"
#include "inc/_byumlibc.h"

#include <string.h>

yumlibc_cfun lstring_t lstring_from_string(const char *src) {
  lstring_t lstring = {
    .start = src,
    .length = strlen(src),
    .owns = 0,
  };

  return lstring; // Idk why this function actually exists... Like people can do it by themselves! (jk jk)
}

yumlibc_cfun const char *lstring_to_string(const lstring_t lstring) {
  char *buff = (char*)yumalloc(lstring.length * sizeof(char));
  for (uint64_t i = 0; i < lstring.length; i++) buff[i] = lstring.start[i];
  return buff;
}

yumlibc_cfun lstring_t surelstring_from_string(const char *src) {
  integer_t length = strlen(src);
  lstring_t lstring = {
    .start  = (const char*)yumalloc(length),
    .length = length,
    .owns   = yumtrue
  };

  return lstring;
}