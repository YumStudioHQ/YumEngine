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

#include "inc/types/base/types.h"
#include "inc/types/system/err.h"
#include "inc/utils/ystringutils.h"
#include "inc/utils/ystringutils.hpp"
#include "inc/yumem.hpp"
#include "inc/managers/lstring_utils.h"

#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>

using namespace YumEngine::xV1;

yumlibc_cfun const char *category_to_ascii(syserr_t err) {
  thread_local std::string _mstr = "unknown error: #";
  _mstr = "unknown error: #";
  switch (err.category) {
    case err.OK: return "ok";
    case err.ERROR: return "error";
    case err.UNKNOWN_ERROR: return "unknown error";
    case err.INVALID_POINTER: return "invalid pointer";
    case err.FILE_NOT_FOUND: return "file not found";
    case err.NOT_A_TABLE: return "not a table";
    case err.INVALID_TYPE: return "invalid type";
    case err.NULL_OR_EMPTY_ARGUMENT: return "null or invalid argument";
    case err.LUA_EXECUTION_ERROR: return "lua execution error";
    case err.ILL_FUNCTION_PATH: return "ill function path";
    case err.PROMOTED_CXX_EXCEPTION: return "promoted C++ exception";
    default: _mstr += std::to_string((int)err.category);
             return _mstr.c_str();
  }
}

yumlibc_cfun lstring_t yumfmterr(syserr_t err) {
  std::ostringstream oss;
  oss << category_to_ascii(err) << '#' << (int)err.category << " : " << lstring2cxxstring(err.comment)
      << "\nfrom " << 
        lstring2cxxstring(err.source.file)
        << ':' << err.source.line << '.'
        << lstring2cxxstring(err.source.func);
  return cxxstring2lstring(oss.str());
}

yumlibc_cfun void yumprinterr(syserr_t err) {
  lstring_t lstr = yumfmterr(err);

  printf("%.*s\n", (int)lstr.length, lstr.start);

  free_lstring(lstr);
}