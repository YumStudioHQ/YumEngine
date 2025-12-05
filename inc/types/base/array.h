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

#ifndef YUM_INCLUDE_GUARD_ARRAY_H
#define YUM_INCLUDE_GUARD_ARRAY_H

#include "types.h"
#include "vardef.h"

typedef struct {
  variant_t *var;
  integer_t  length;
  boolean_t  owns;
} vararray_t;

#endif // !YUM_INCLUDE_GUARD_ARRAY_H