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

#ifndef YUM_INCLUDE_GUARD_YUM_C_MEMORY_H
#define YUM_INCLUDE_GUARD_YUM_C_MEMORY_H

#include "_byumlibc.h"

yumlibc_cfun void *yumalloc(unsigned long long int s);
yumlibc_cfun void  yumfree(void *p);
yumlibc_cfun char *yumstrcpy(const char *src, unsigned long long int len);

#endif // YUM_INCLUDE_GUARD_YUM_C_MEMORY_H