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
#include "inc/types/system/err.h"
#include "inc/utils/ystringutils.h"
#include "inc/utils/ystringutils.hpp"
#include "inc/types/system/exception.hpp"
#include "inc/types/containers/string.hpp"

#include <vector>
#include <unordered_map>

namespace YumEngine::xV1 {
  static variant_t variant_from_lua(lua_State *L, int idx) {
    int type = lua_type(L, idx);
    switch (type) {
      case LUA_TBOOLEAN: return Variant((boolean_t)lua_toboolean(L, idx)).c();
      case LUA_TSTRING: {
        size_t len;
        const char *cstr = lua_tolstring(L, idx, &len);
        return Variant(lstring_t{.start = cstr, .length = len, .owns = false});
      }
      case LUA_TNUMBER: {
        if (lua_isinteger(L, idx)) {
          return Variant((integer_t)lua_tointeger(L, idx));
        } return Variant((number_t)lua_tonumber(L, idx));
      }
      case LUA_TTABLE: {
        // Check if it's a UID table
        lua_getfield(L, idx, "__yumuid");
        if (!lua_isnil(L, -1)) {
          uint64_t bytes = lua_tointeger(L, -1);
          lua_pop(L, 1);
          return Variant(vuid_t{.bytes = bytes});
        }

        lua_pop(L, 1); // pop nil
        // Check if it's a binary table
        lua_getfield(L, idx, "__yumbytes");
        if (!lua_isnil(L, -1)) {
          size_t len;
          const char *data = lua_tolstring(L, -1, &len);
          lua_pop(L, 1);
          return Variant(binary_t{.start = (const uint8_t*)data, .length = len, .owns = false});
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

  static void push_vararray_to_lua(lua_State *L, vararray_t array) {
    for (integer_t i = 0; i < array.length; i++) {
      push_variant_to_lua(L, array.var[i]);
    }
  }

  State::State() {
    L = luaL_newstate();
  }

  State::~State() {
    lua_close(L);
  }

  void State::push_callback(ascii name, const yum_callback &callback) {
    if (!name) yumlibcxx_throw(expected a function name, syserr_t::NULL_OR_EMPTY_ARGUMENT, argument const lstring &name);

    thread_local std::unordered_map<std::string, yum_callback> _callbacks;
    _callbacks[name] = callback;

    thread_local auto static_lua_callback = [](lua_State *L) -> int {
      const char *cstrname = lua_tostring(L, lua_upvalueindex(1));
      if (!cstrname) return 0;

      auto it = _callbacks.find(cstrname);
      if (it == _callbacks.end()) return 0;
      
      int nargs = lua_gettop(L);
      vararray_t arguments_from_lua = {
        .var    = (variant_t*)yumalloc(sizeof(variant_t) * nargs),
        .length = nargs,
        .owns   = true
      };

      for (int i = 1; i < nargs; i++)
        arguments_from_lua.var[i] = variant_from_lua(L, i);
      
      vararray_t result = it->second(arguments_from_lua);
      push_vararray_to_lua(L, result);
      return result.length;
    };

    lua_pushstring(L, name);
    lua_pushcclosure(L, static_lua_callback, 1);
    lua_setfield(L, -2, name);
    lua_pop(L, 1);
  }

  vararray_t State::call(const lstring_t &path, const vararray_t &args) {
    lua_getglobal(L, "_G");

    vararray_t arguments_from_lua = { };
    bool found_function = false;

    Sdk::strview(path.start, path.length).split('.', [&](Sdk::strview view) {
      if (found_function)
        yumlibcxx_throw("Ill formed function path", syserr_t::ILL_FUNCTION_PATH, function already called (found_function == true))

      lua_getfield(L, -1, view.head());
      lua_remove(L, -2);

      if (lua_isfunction(L, -1)) {
        found_function = true;

        if (lua_pcall(L, args.length, LUA_MULTRET, 0) != LUA_OK) {
          yumlibcxx_throw(lua_tostring(L, -1), syserr_t::LUA_EXECUTION_ERROR, "when calling lua function from C++");
          // For later: Why not implement a traceback ?
        } else {
          int nargs = lua_gettop(L);

          arguments_from_lua.var = (variant_t*)yumalloc(sizeof(variant_t) * nargs);
          arguments_from_lua.length = nargs;
          arguments_from_lua.owns = true;

          for (int i = 1; i <= nargs; i++)
            arguments_from_lua.var[i-1] = variant_from_lua(L, i);
        }
      }

      if (!lua_istable(L, -1))
        yumlibcxx_throw("expected table", syserr_t::INVALID_TYPE, lua_istable(L, -1) returned false);
    });
    
    return arguments_from_lua;
  }

  void State::push_variant(ascii name, const variant_t &var) {
    push_variant_to_lua(L, var);
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
    if (isfile) {
      if (luaL_dofile(L, source) != LUA_OK)
        return yummakeerror_runtime(lua_tostring(L, -1), syserr_t::LUA_EXECUTION_ERROR);
    } else {
      if (luaL_dostring(L, source) != LUA_OK)
        return yummakeerror_runtime(lua_tostring(L, -1), syserr_t::LUA_EXECUTION_ERROR);
    }

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