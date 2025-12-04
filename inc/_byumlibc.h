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

#ifndef YUM_INCLUDE_GUARD_BACKEND_YUMLIB_C_H
#define YUM_INCLUDE_GUARD_BACKEND_YUMLIB_C_H

#ifdef __cplusplus
#define yumlibc_cfun extern "C"
#else
#define yumlibc_cfun extern 
#endif // __cplusplus

#ifdef _WIN32
#  define yumlibc_dllattribute __declspec(dllexport)
#else
#  define yumlibc_dllattribute __attribute__((visibility("default")))
#endif

#define yumlibc_vdllmember yumlibc_cfun yumlibc_dllattribute

#define yumtrue   1
#define yumfalse  0

#endif // YUM_INCLUDE_GUARD_BACKEND_YUMLIB_C_H