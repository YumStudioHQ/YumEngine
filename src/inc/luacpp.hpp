/*********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *            This file is free & open source            *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                          from                         *
 *                         MONOE                         *
 *                                                       *
 *********************************************************/
#pragma once

#include <iomanip>

#include "lua.hpp"

#include "glob.hpp"

namespace YumEngine {
  /**
   * @brief Holds a `lua_State*`, and frees it when destroyed.
   */
  class LuaCxx {
  private:
    lua_State *state;
  
  public:
    inline LuaCxx(bool libs = true) : state(luaL_newstate()) {
      if (libs) luaL_openlibs(state);
    }

    inline lua_State *get() { return state; }

    inline ~LuaCxx() {
      if (state) {
        ((*G_out()) << "yum: destroying lua_State 0x" << std::hex << std::setw(16) << std::setfill('0') << state << std::endl);
        lua_close(state);
      }
    }
  };
}