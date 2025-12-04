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

#include "inc/api/ystatec.h"
#include "inc/types/variant.h"
#include "inc/types/base/array.h"
#include "inc/types/state.hpp"
#include "inc/types/system/err.h"
#include "inc/types/system/exception.hpp"

using namespace YumEngine::xV1;

yumlibcxx_c_header_decoration_begin

syserr_t yum_move_cxx_function(push_callback)(YumState *state, const lstring_t *name, const yumcallback_t callback) {
  try {
    state->push_callback(*name, callback);
  } catch (const sysexception &e) {
    return e.geterr();
  }

  return yumsuccess;
}

syserr_t  yum_move_cxx_function(call)(YumState *state, const lstring_t *path, const vararray_t *args, vararray_t *out) {
  if (!out) {
    return yummakeerror("*out is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  }

  try {
    (*out) = state->call(*path, *args);
  } catch (const sysexception &e) {
    return e.geterr();
  }

  return yumsuccess;
}

syserr_t  yum_move_cxx_function(push_variant)(YumState *state, ascii name, const variant_t *var);
syserr_t  yum_move_cxx_function(push_table)(YumState *state, ascii name);
syserr_t  yum_move_cxx_function(new_table)(YumState *state, ascii name);
syserr_t  yum_move_cxx_function(run)(YumState *state, ascii source, boolean_t isfile);
syserr_t  yum_move_cxx_function(load)(YumState *state, const lstring_t *source, boolean_t isfile);
void      yum_move_cxx_function(clear)(YumState *state);

yumlibcxx_c_header_decoration_end