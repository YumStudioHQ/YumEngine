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
#include "base/array.h"
#include "system/err.h"

#include <string>

namespace YumEngine::xV1 {
  /** @brief Represents a Yum callback. Takes an array of variant as input, and returns an array of variant. */
  typedef vararray_t (*yum_callback)(vararray_t);

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
     * @return Error state (syserr type).
     */
    syserr_t push_callback(const lstring_t &name, const yum_callback &callback);

    /**
     * @brief Calls a Lua function.
     * @param path The path of the function in a string (e.g. sometable.anotherone.funcname)
     * @param args Arguments that you will give to the function.
     * @warning path **should not** contain null characters, and **should end** with a null-terminator.
     */
    vararray_t call(const lstring_t &path, const vararray_t &args);

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
     * @brief Runs a string or a file
     * @param source Path to the file or a string containing Lua code.
     * @return Error state (syserr type).
     */
    syserr_t run(const lstring_t &source, boolean_t isfile);

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
  };
}
