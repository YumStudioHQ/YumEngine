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
#include "utils/ystringutils.h"
#include <stdexcept>

namespace YumEngine::xV1 {
  class sysexception : public std::exception, public syserr_t {
  private:  syserr_t    err = {
              .category = err.UNKNOWN_ERROR,
              .source = { .file = lstring_from_string("uknown"), .func = lstring_from_string("unknown"), .line = -1},
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

#define yumlibcxx_throw(what, kind, ...) throw yummakeerror_x(what, kind, __VA_ARGS__)