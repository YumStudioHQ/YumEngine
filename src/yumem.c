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

void *yumalloc(unsigned long long int s) {
  return malloc(s);
}

void yumfree(void *p) {
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
