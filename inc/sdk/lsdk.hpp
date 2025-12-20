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

#include "inc/types/state.hpp"
#include "inc/sdk/lbuffer.hpp"
#include "inc/sdk/lstring.hpp"
#include "inc/types/variant.hpp"

#include <functional> // Include std::function<R(Args...)> type.

namespace YumEngine::xV1::Sdk {

  typedef Buffer<CVariant>(*SdkCallback)(Buffer<CVariant>);

  /**
   * @brief A higher-level implementation of State. Manages a Lua state.
   */
  class SdkState {
  private:
    State mstate;

  public:
    /**
     * @brief Calls a Lua function.
     * 
     * @param name The name of the function.
     * @param buff Arguments of the call.
     * @return The returned values of the Lua function in a buffer. The Lua function cannot return a table!
     */
    containers::list<CVariant> call(const StringView &name, const Buffer<CVariant> &buff);

    /**
     * @brief Calls a Lua function without passing any arguments.
     * 
     * @param name The name of the function.
     * @return The returned values of the Lua function in a buffer. The Lua function cannot return a table!
     */
    containers::list<CVariant> call(const StringView &name);

    /**
     * @brief Pushes a value inside the Lua VM.
     * 
     * @param name The name of the value.
     * @param var A Variant value.
     */
    void                       push(const StringView &name, const CVariant &var);

    /**
     * @brief Pushes a callback to the Lua VM.
     * 
     * @param name The name of the function.
     * @param callback A function pointer.
     */
    void                       push_callback(const StringView &name, yum_callback callback);

    /**
     * @brief Pushes a callback to the Lua VM.
     * 
     * @param name The name of the function.
     * @param callback A function.
     */
    void                       push_callback(const StringView &name, SdkCallback callback);

    /**
     * @brief Opens Lua's standard library.
     */
    void                       open_libs();

    /**
     * @brief Runs a lua code.
     * 
     * @param code The code or the file name.
     * @param isFile True when it is a file.
     */
    void                       run(const StringView &code, bool isFile = false);
  };
}