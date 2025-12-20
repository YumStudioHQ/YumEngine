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

#include <stdlib.h>

#include "inc/types/base/types.h"
#include "inc/types/base/vardef.h"
#include "inc/_byumlibc.h"

yumlibc_vdllmember void *yumalloc(uint64_t s) {
  return malloc(s);
}

yumlibc_vdllmember void yumfree(void *p) {
  free(p); 
}

lstring_t alloc_lstring(integer_t size) {
  lstring_t lstring = {
    .start = yumalloc(size * sizeof(char)),
    .length = size,
    .owns = 1
  };

  return lstring;
}

void free_lstring(lstring_t lstring) {
  if (lstring.owns && lstring.start && lstring.length > 0) {
    yumfree((void*)lstring.start);
  }
}

// We can technically just cast it as an lstring_t... But let's make things proper!
void free_binary(binary_t bin) {
  if (bin.owns && bin.start && bin.length > 0) {
    yumfree((void*)bin.start);
  }
}

// TODO: make it visible in header files.
yumlibc_vdllmember void yumfree_lstring(lstring_t lstring) {
  free_lstring(lstring);
}

yumlibc_vdllmember void yumfree_all(variant_t *vars, uint64_t length) {
  for (uint64_t i = 0; i < length; i++) {
    variant_t var = vars[i];
    if (var.type == VARIANT_STRING) yumfree_lstring(var.hold.lstring);
    else if (var.type == VARIANT_BINARY) free_binary(var.hold.binary);
  }
}

yumlibc_vdllmember void yumfree_array(variant_t *vars, uint64_t length) {
  yumfree_all(vars, length);
  yumfree((void*)vars);
}

yumlibc_vdllmember char *yumstrcpy(const char *src, uint64_t len) {
  char *buff = (char*)yumalloc(sizeof(char) * len);
  for (uint64_t i = 0; i < len; i++) buff[i] = src[i];
  return buff;
}