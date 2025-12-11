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

#ifdef DEBUG
#include <stdio.h>
#define YUM_DEBUG_HERE      printf("YUM-DEBUG-POINT-REACHED:\t%s:%u:%s\n", __FILE__, __LINE__, __func__);
#define YUM_DEBUG_PUTS(w)   printf("YUM-DEBUG-POINT-REACHED:\t%s:%u:%s:%s\n", __FILE__, __LINE__, __func__, w);
#define YUM_DEBUG_PUTL(s,l) printf("YUM-DEBUG-POINT-REACHED:\t%s:%u:%s:'%.*s'\n", __FILE__, __LINE__, __func__, l, s);
#define YUM_DEBUG_OUTF      printf("YUM-DEBUG-POINT-REACHED:\t%s:%u:%s: exiting function\n", __FILE__, __LINE__, __func__);
#define YUM_DEBUG_CHECKP(n) printf("YUM-DEBUG-POINT-REACHED:\t%s:%u:%s: check-point '%s'\n", __FILE__, __LINE__, __func__, n);
#else
#define YUM_DEBUG_HERE
#define YUM_DEBUG_PUTS(w)
#define YUM_DEBUG_PUTL(s,l)
#define YUM_DEBUG_OUTF
#define YUM_DEBUG_CHECKP(n)
#endif