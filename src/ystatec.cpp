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
#include "inc/types/state.hpp"
#include "inc/types/system/err.h"
#include "inc/types/system/exception.hpp"

#include <typeinfo>

using namespace YumEngine::xV1;

yumlibcxx_c_header_decoration_begin

YumState *yumlibc_library_member(new)() {
  return new YumState();
}

void yumlibc_library_member(delete)(const YumState *state) {
  if (state) delete state;
}

void yumlibc_library_member(open_libs)(YumState *state) {
  if (state) state->open_stdlibs();
}

syserr_t yumlibc_library_member(push_callback)(YumState *state, ascii name, const yum_callback callback) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  try {
    state->push_callback(name, callback);
  } catch (const sysexception &e) {
    return e.geterr();
  } catch (const std::exception &e) {
    return yumlibcxx_promote_this_exception(e);
  }

  return yumsuccess;
}

syserr_t yumlibc_library_member(call)(YumState *state, ascii path, uint64_t pathlen, uint64_t argc, const variant_t *argv, variant_t *out, uint64_t *outc) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  if (!path || pathlen == 0) {
    return yummakeerror("(lstring_t)path is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  }

  try {
    return state->call(path, pathlen, argc, argv, *outc, out);
  } catch (const sysexception &e) {
    return e.geterr();
  } catch (const std::exception &e) {
    return yumlibcxx_promote_this_exception(e);
  }

  return yumsuccess;
}

syserr_t yumlibc_library_member(push_variant)(YumState *state, ascii name, const variant_t *var) {
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
  } catch (const std::exception &e) {
    return yumlibcxx_promote_this_exception(e);
  }

  return yumsuccess;
}

syserr_t yumlibc_library_member(push_table)(YumState *state, ascii name) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  if (!name) {
    return yummakeerror("(ascii)name is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  }

  try {
    state->push_table(name);
  } catch (const sysexception &e) {
    return e.geterr();
  } catch (const std::exception &e) {
    return yumlibcxx_promote_this_exception(e);
  }

  return yumsuccess;
}

void yumlibc_library_member(push_global)(YumState *state, ascii name) {
  if (state) state->push_global(name);
}

syserr_t yumlibc_library_member(new_table)(YumState *state, ascii name) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  if (!name) {
    return yummakeerror("(ascii)name is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);
  }

  try {
    state->new_table(name);
  } catch (const sysexception &e) {
    return e.geterr();
  } catch (const std::exception &e) {
    return yumlibcxx_promote_this_exception(e);
  }

  return yumsuccess;
}

syserr_t yumlibc_library_member(run)(YumState *state, ascii source, boolean_t isfile) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);

  return state->run(source, isfile);
}

syserr_t yumlibc_library_member(load)(YumState *state, const lstring_t *source, boolean_t isfile) {
  if (!state) return yummakeerror("(YumState*)state pointer is null", syserr_t::NULL_OR_EMPTY_ARGUMENT);

  return state->load(*source, isfile);
}

void yumlibc_library_member(clear)(YumState *state) {
  if (state) {
    state->clear();
  }
}

yumlibcxx_c_header_decoration_end