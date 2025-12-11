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

#include "variant.h"
#include "variant.hpp"
#include "lua/lua.hpp"
#include "base/types.h"
#include "base/callbacks.h"
#include "system/err.h"

#include <string>

namespace YumEngine::xV1 {
  /** @brief Represents a Yum callback. Takes an array of variant as input, and returns an array of variant. */
  using yum_callback = yumlibc_callback_type;

  class State {
  private:
    lua_State *L;

  public:
    /** @brief Initializes a new State. */
    State();

    /** @brief Destroys the current State. */
    ~State();

    /** 
     * @brief Pushes a C-like callback to the State. 
     * @param name The name of the callback.
     * @param callback The callback that will get pushed.
     * @note You may push a table before pushing a callback (or you will end with UBs!)
     */
    void push_callback(ascii name, const yum_callback &callback);

    /**
     * @brief Calls a Lua function.
     * @param path The path of the function in a string (e.g. sometable.anotherone.funcname)
     * @param pathlen Size of the path.
     * @param argc Count of arguments.
     * @param argv Arguments that you will give to the function.
     * @param outc [Out] count of returned arguments.
     * @param out [Out] output of the call.
     * @warning path **should not** contain null characters, and **should end** with a null-terminator.
     */
    syserr_t call(ascii path, uint64_t pathlen, uint64_t argc, const variant_t* args, uint64_t& nargs, variant_t **out);

    /**
     * @brief Pushes a value.
     * @param name The name of value.
     * @param var  A variant. Will push its holded value.
     * @note You might free this by yourself in higher languages. (Depending on how you call functions)
     */
    void push_variant(ascii name, const variant_t &var);

    /**
     * @brief Changes the stack to the table, and if not created, creates it.
     * @param name The name of the table.
     */
    void push_table(ascii name);

    /**
     * @brief Creates a new table.
     * @param name The name of the table.
     */
    void new_table(ascii name);

    /**
     * @brief Pushes a global value onto the stack.
     * @param name the name of the global value.
     */
    void push_global(ascii name);

    /**
     * @brief Runs a string or a file
     * @param source Path to the file or a string containing Lua code.
     * @return Error state (syserr type).
     */
    syserr_t run(ascii source, boolean_t isfile);

    /**
     * @brief Loads a string or a file
     * @param source Path to the file or a string containing Lua code.
     * @return Error state (syserr type).
     */
    syserr_t load(const lstring_t &source, boolean_t isfile);

    /**
     * @brief Clears the internal Lua stack.
     * @note You may call this function when catching an exception.
     */
    void clear();

    /** @brief Opens Lua's standard libraries. */
    void open_stdlibs();
  };
}

using YumState = YumEngine::xV1::State;