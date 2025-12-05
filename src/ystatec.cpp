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

#include "inc/types/variant.h"
#include "inc/types/base/array.h"
#include "inc/types/state.hpp"
#include "inc/types/system/err.h"
#include "inc/types/system/exception.hpp"

using namespace YumEngine::xV1;

yumlibcxx_c_header_decoration_begin

YumState *yum_move_cxx_function(new)() {
  return new YumState();
}

void yum_move_cxx_function(delete)(const YumState *state) {
  if (state) delete state;
}

syserr_t yum_move_cxx_function(push_callback)(YumState *state, const lstring_t *name, const yum_callback callback) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  try {
    state->push_callback(*name, callback);
  } catch (const sysexception &e) {
    return e.geterr();
  }

  return yumsuccess;
}

syserr_t yum_move_cxx_function(call)(YumState *state, const lstring_t *path, const vararray_t *args, vararray_t *out) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  if (!path->start || path->length == 0) {
    return yummakeerror("(lstring_t)path is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  } else if (!out) {
    return yummakeerror("*out is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  }

  try {
    (*out) = state->call(*path, *args);
  } catch (const sysexception &e) {
    return e.geterr();
  }

  return yumsuccess;
}

syserr_t yum_move_cxx_function(push_variant)(YumState *state, ascii name, const variant_t *var) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  if (!name) {
    return yummakeerror("(ascii)name is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  } else if (!var) {
    return yummakeerror("*var is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  }

  try {
    state->push_variant(name, *var);
  } catch (const sysexception &e) {
    return e.geterr();
  }

  return yumsuccess;
}

syserr_t yum_move_cxx_function(push_table)(YumState *state, ascii name) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  if (!name) {
    return yummakeerror("(ascii)name is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  }

  try {
    state->push_table(name);
  } catch (const sysexception &e) {
    return e.geterr();
  }

  return yumsuccess;
}

syserr_t yum_move_cxx_function(new_table)(YumState *state, ascii name) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  if (!name) {
    return yummakeerror("(ascii)name is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  }

  try {
    state->new_table(name);
  } catch (const sysexception &e) {
    return e.geterr();
  }

  return yumsuccess;
}

syserr_t yum_move_cxx_function(run)(YumState *state, ascii source, boolean_t isfile) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);

  return state->run(source, isfile);
}

syserr_t yum_move_cxx_function(load)(YumState *state, const lstring_t *source, boolean_t isfile) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);

  return state->load(*source, isfile);
}

void yum_move_cxx_function(clear)(YumState *state) {
  if (state) {
    state->clear();
  }
}

yumlibcxx_c_header_decoration_end