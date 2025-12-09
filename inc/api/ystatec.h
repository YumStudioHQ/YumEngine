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

#ifndef YUM_INCLUDE_GUARD_STATE_C_H
#define YUM_INCLUDE_GUARD_STATE_C_H

#include "_byumlibc.h"
#include "inc/types/base/types.h"
#include "inc/types/base/vardef.h"
#include "inc/types/system/err.h"
#include "inc/types/base/callbacks.h"

typedef yumlibc_callback_type yumcallback_t;
typedef struct YumState YumState;

yumlibcxx_c_header_decoration_begin

YumState *yumlibc_library_member(new)(void);
void      yumlibc_library_member(delete)(const YumState *state);
syserr_t  yumlibc_library_member(push_callback)(YumState *state, ascii name, const yumcallback_t callback);
syserr_t  yumlibc_library_member(call)(YumState *state, ascii path, uint64_t pathlen, uint64_t argc, const variant_t *argv, variant_t *out, uint64_t *outc);
syserr_t  yumlibc_library_member(push_variant)(YumState *state, ascii name, const variant_t *var);
syserr_t  yumlibc_library_member(push_table)(YumState *state, ascii name);
syserr_t  yumlibc_library_member(new_table)(YumState *state, ascii name);
syserr_t  yumlibc_library_member(run)(YumState *state, ascii source, boolean_t isfile);
syserr_t  yumlibc_library_member(load)(YumState *state, const lstring_t *source, boolean_t isfile);
void      yumlibc_library_member(clear)(YumState *state);

yumlibcxx_c_header_decoration_end

#endif // !YUM_INCLUDE_GUARD_STATE_C_H