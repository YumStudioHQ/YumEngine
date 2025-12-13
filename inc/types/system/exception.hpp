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

#pragma once

#include "err.h"
#include "managers/lstring_utils.h"
#include "utils/ystringutils.hpp"
#include "utils/ystringutils.h"
#include <stdexcept>

namespace YumEngine::xV1 {
  class sysexception : public std::exception, public syserr_t {
  private:  syserr_t    err = {
              .category = err.UNKNOWN_ERROR,
              .source = { .func = lstring_from_string("unknown"), .file = lstring_from_string("uknown"), .line = -1},
              .comment = lstring_from_string("unknown exception")
            };
            
            lstring_t   fmtmsg;

  public:   inline sysexception() {}

            inline sysexception(const syserr_t &_err)
              : err(_err) {
                fmtmsg = yumfmterr(err);
              }
    
            inline const char *what() const noexcept override {
              return fmtmsg.start;
            }

            inline ~sysexception() {
              free_lstring(fmtmsg);
            }

            inline syserr_t geterr() const { return err; }
  };
}

#define yumlibcxx_throw(what, kind, ...) throw sysexception(yummakeerror_x(what, kind, __VA_ARGS__))

#define yumlibcxx_make_exception_from(syserr) throw YumEngine::xV1::sysexception(syserr)
#define yumlibcxx_promote_this_exception(e) syserr_t{ \
    .category = syserr_t::PROMOTED_CXX_EXCEPTION, \
    .source = { \
      .func = lstring_from_string(__func__), \
      .file = lstring_from_string(__FILE__), \
      .line = __LINE__, \
    }, \
    .comment = YumEngine::xV1::cxxstring2lstring(e.what() + std::string(" * ") + typeid(e).name()) \
  }