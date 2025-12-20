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

#include "inc/yumem.hpp"
#include "inc/lua/lua.hpp"
#include "inc/types/state.hpp"
#include "inc/types/variant.h"
#include "inc/types/variant.hpp"
#include "inc/debug/dbgpoints.h"
#include "inc/types/system/err.h"
#include "inc/utils/ystringutils.h"
#include "inc/utils/ystringutils.hpp"
#include "inc/types/system/exception.hpp"
#include "inc/types/containers/string.hpp"

#include <vector>
#include <unordered_map>

namespace YumEngine::xV1 {
  namespace _static_units {
    /**
     * @brief Internal function : Converts Lua variables to C values.
     * 
     * @warning You may free returned values! You can use the yumfree_array() function (or yumfree for C++) since the 3.5 (coming with the C++ SDK).
     * @param L lua state.
     * @param idx Index.
     * @return A C variant.
     */
    static variant_t variant_from_lua(lua_State *L, int idx) {
      int type = lua_type(L, idx);
      switch (type) {
        case LUA_TBOOLEAN: return CVariant((boolean_t)lua_toboolean(L, idx)).c();
        case LUA_TSTRING: {
          size_t len;
          const char *cstr = lua_tolstring(L, idx, &len);
          lstring_t lstring {
            .start = yumstrcpy(cstr, len),
            .length = len, 
            .owns = true
          };

          return CVariant(lstring);
        }
        case LUA_TNUMBER: {
          if (lua_isinteger(L, idx)) {
            return CVariant((integer_t)lua_tointeger(L, idx));
          } return CVariant((number_t)lua_tonumber(L, idx));
        }
        case LUA_TTABLE: {
          // Check if it's a UID table
          lua_getfield(L, idx, "__yumuid");
          if (!lua_isnil(L, -1)) {
            uint64_t bytes = lua_tointeger(L, -1);
            lua_pop(L, 1);
            return CVariant(vuid_t{.bytes = bytes});
          }

          lua_pop(L, 1); // pop nil
          // Check if it's a binary table
          lua_getfield(L, idx, "__yumbytes");
          if (!lua_isnil(L, -1)) {
            size_t len;
            const char *data = yumstrcpy(lua_tolstring(L, -1, &len), len);
            lua_pop(L, 1);
            return CVariant(binary_t{.start = (const uint8_t*)data, .length = len, .owns = true});
          }
          lua_pop(L, 1); // pop nil

          // Well tables not for now.
          return variant_t{.type = variant_t::VARIANT_NIL};
        }

        default: return variant_t{.type = variant_t::VARIANT_NIL};
      }
    }

    static void push_variant_to_lua(lua_State *L, const variant_t &var) {
      switch (var.type) {
        case variant_t::VARIANT_INTEGER: 
          lua_pushinteger(L, var.hold.integer);
          break;
        case variant_t::VARIANT_NUMBER: 
          lua_pushnumber(L, var.hold.number);
          break;
        case variant_t::VARIANT_BOOL: 
          lua_pushboolean(L, var.hold.boolean);
          break;
        case variant_t::VARIANT_STRING: 
          lua_pushlstring(L, var.hold.lstring.start, var.hold.lstring.length);
          break;
        case variant_t::VARIANT_BINARY: 
          lua_newtable(L);
          lua_pushstring(L, "__yumbin");
          lua_pushlstring(L, (const char*)var.hold.binary.start, var.hold.binary.length);
          lua_settable(L, -3);
          break;
        case variant_t::VARIANT_UID: 
          lua_newtable(L);
          lua_pushstring(L, "__yumuid");
          lua_pushinteger(L, var.hold.uid.bytes);
          lua_settable(L, -3);
          break;
        default: lua_pushnil(L); break;
      }
    }

    static void push_vararray_to_lua(lua_State *L, uint64_t argc, const variant_t *argv) {
      for (uint64_t i = 0; i < argc; i++) {
        push_variant_to_lua(L, argv[i]);
      }
    }


    thread_local std::unordered_map<std::string, yum_callback> _callbacks;

    static void dump_lua(lua_State *L) {
      ascii code = "for k, v in pairs(_G) do"
                   "  print(k, v)"
                   "end";
      luaL_dostring(L, code);
    }

    static int static_lua_callback(lua_State* L) {
      const char* cstrname = lua_tostring(L, lua_upvalueindex(1));
      if (!cstrname) return 0;

      auto it = _callbacks.find(cstrname);
      if (it == _callbacks.end()) return 0;

      int nargs = lua_gettop(L);
      variant_t* arguments_from_lua = (variant_t*)yumalloc(sizeof(variant_t) * nargs);

      for (int i = 0; i < nargs; i++) {
        arguments_from_lua[i] = variant_from_lua(L, i + 1);
      }

      uint64_t outc;
      variant_t* result = it->second(nargs, arguments_from_lua, &outc);
      
      push_vararray_to_lua(L, outc, result);
      
      yumfree((void*)arguments_from_lua);
      // TODO: queue_free?
      
      return static_cast<int>(outc);
    }

    void cd(lua_State *L, const Sdk::strview &view) {
      YUM_DEBUG_HERE
      lua_getglobal(L, "_G");

      view.split('.', [&L](Sdk::strview key_view) {
        lua_pushlstring(L, key_view.head(), key_view.length());
        lua_gettable(L, -2);
        lua_remove(L, -2);

        if (lua_isnil(L, -1)) {
          std::runtime_error err("got nil (pushed table is nil) when pushing: " + std::string(key_view.head(), key_view.length()));
          YUM_DEBUG_CALL(dump_lua(L))
          yumlibcxx_make_exception_from(yumlibcxx_promote_this_exception(err));
        }
      });
      YUM_DEBUG_OUTF
    }
  }

  State::State() {
    YUM_DEBUG_HERE
    L = luaL_newstate();
  }

  State::~State() {
    lua_close(L);
  }

  void State::push_callback(ascii name, const yum_callback &callback) {
    YUM_DEBUG_HERE

    if (!name) yumlibcxx_throw(expected a function name, syserr_t::NULL_OR_EMPTY_ARGUMENT, argument const lstring &name);
    
    _static_units::_callbacks[name] = callback;
    
    int top_before = lua_gettop(L);

    lua_pushstring(L, name);
    lua_pushcclosure(L, _static_units::static_lua_callback, 1);
    lua_setfield(L, -2, name);
    lua_settop(L, top_before);
    
    YUM_DEBUG_OUTF
  }

  syserr_t State::call(ascii path, uint64_t pathlen, uint64_t argc, const variant_t* args, uint64_t& nargs, variant_t** out) {
    YUM_DEBUG_HERE;

    nargs = 0;

    int top_before = lua_gettop(L);

    // Push function onto stack
    _static_units::cd(L, Sdk::strview(path, pathlen));
    YUM_DEBUG_HERE

    if (!lua_isfunction(L, -1)) {
      lua_settop(L, top_before);
      return yummakeerror_runtime("Not a Lua function", syserr_t::LUA_EXECUTION_ERROR);
    }

    // Push args AFTER function is already at stack top
    _static_units::push_vararray_to_lua(L, argc, args);

    // Call
    YUM_DEBUG_PUTS("calling lua function")
    if (lua_pcall(L, argc, LUA_MULTRET, 0) != LUA_OK) {
      std::string msg = lua_tostring(L, -1);
      msg += "* when calling: `" + std::string(path) + "`";

      lua_settop(L, top_before);

      return syserr_t{
        .category = syserr_t::LUA_EXECUTION_ERROR,
        .source   = { .file = lstring_from_string(__FILE__),
                      .func = lstring_from_string(__func__),
                      .line = __LINE__ },
        .comment  = cxxstring2lstring(msg)
      };
    }

    // Calculate returned values
    int top_after = lua_gettop(L);
    nargs = top_after - top_before;
    YUM_DEBUG_PUTS("translating args")
    if (nargs > 0) {
      YUM_DEBUG_HERE
      (*out) = (variant_t*)yumalloc(nargs * sizeof(variant_t));
      YUM_DEBUG_HERE
      int first_ret = top_after - nargs + 1;
      YUM_DEBUG_HERE
      for (uint64_t i = 0; i < nargs; ++i) {
        (*out)[i] = _static_units::variant_from_lua(L, first_ret + i);
      }
    }

    lua_settop(L, top_before);
    YUM_DEBUG_OUTF
    return yumsuccess;
  }

  void State::push_variant(ascii name, const variant_t &var) {
    _static_units::push_variant_to_lua(L, var);
    lua_setfield(L, -2, name);
    lua_pop(L, 1);
  }

  void State::new_table(ascii name) {
    lua_newtable(L);
    lua_setfield(L, -2, name);
  }

  void State::push_table(ascii name) {
    lua_getfield(L, -1, name);
  }

  void State::push_global(ascii name) {
    lua_getglobal(L, name);
  }

  syserr_t State::run(ascii source, boolean_t isfile) {
    YUM_DEBUG_HERE
    if (isfile) {
      if (luaL_dofile(L, source) != LUA_OK)
        return yummakeerror_runtime(lua_tostring(L, -1), syserr_t::LUA_EXECUTION_ERROR);
    } else {
      if (luaL_dostring(L, source) != LUA_OK)
        return yummakeerror_runtime(lua_tostring(L, -1), syserr_t::LUA_EXECUTION_ERROR);
    }

    YUM_DEBUG_OUTF
    return yumsuccess;
  }

  syserr_t State::load(const lstring_t &source, boolean_t isfile) {
    if (isfile) {
      if (luaL_loadfile(L, source.start) != LUA_OK)
        return yummakeerror_runtime(lua_tostring(L, -1), syserr_t::LUA_EXECUTION_ERROR);
    } else {
      if (luaL_loadbuffer(L, source.start, source.length, "yumlibcxx_loadbuffer_api") != LUA_OK)
        return yummakeerror_runtime(lua_tostring(L, -1), syserr_t::LUA_EXECUTION_ERROR);
    }

    return yumsuccess;
  }

  void State::clear() {
    lua_pop(L, 0);
  }

  void State::open_stdlibs() {
    luaL_openlibs(L);
  }
}

// TODO: Lua.Debug utilities.