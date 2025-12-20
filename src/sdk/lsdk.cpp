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

#include "inc/types/state.hpp"
#include "inc/sdk/lbuffer.hpp"
#include "inc/sdk/lstring.hpp"
#include "inc/types/variant.hpp"
#include "inc/sdk/lsdk.hpp"

#include "inc/types/system/exception.hpp"

#include <functional> // Include std::function<R(Args...)> type.

namespace YumEngine::xV1::Sdk {
  /* Working on
   * class SdkState; */

  containers::list<CVariant> SdkState::call(const StringView &name, const Buffer<CVariant> &buff) {
    uint64_t nargs;
    variant_t *out;
    containers::list<variant_t> variants;
    variants.reserve(buff.length());
    buff.foreach([&variants](CVariant var) { variants.append(var.c()); });

    syserr_t err = mstate.call(name.utf8(), name.length(), variants.length(), variants.data(), nargs, &out);
    
    if (err.category != err.OK) yumlibcxx_make_exception_from(err);

    containers::list<CVariant> cvars;
    cvars.reserve(nargs);
    containers::memoryslice<variant_t>(out, nargs, false)
              .foreach([&cvars](variant_t var) { cvars.append(var); });
    
    return cvars;
  }

  containers::list<CVariant> SdkState::call(const StringView &name) {
    return call(name, Buffer<CVariant>());
  }

  void SdkState::push(const StringView &name, const CVariant &var) {
    mstate.push_global("_G");
    auto end = name.rfind('.');

    StringView(name.slice(0, end))
              .split('.')
              .foreach([this](StringView view) {
                mstate.push_table(view.move().utf8());
              });
    auto lname = StringView(name.slice(end, name.length() - end)).move().utf8();
    mstate.push_variant(lname, var.c());
  }

  void SdkState::push_callback(const StringView &name, yum_callback callback) {
    mstate.push_callback(name.move().utf8(), callback);
  }

  void SdkState::open_libs() {
    mstate.open_stdlibs();
  }

  void SdkState::run(const StringView &code, bool isFile) {
    mstate.run(code.move().utf8(), isFile);
  }
}