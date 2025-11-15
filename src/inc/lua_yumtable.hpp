/* lua_yumtable.hpp */

/*********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *            This file is free & open source            *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                          from                         *
 *                         MONOE.                        *
 *                                                       *
 *********************************************************/

#pragma once

/**
 * @file lua_yumtable.hpp
 * 
 * @brief Small internal utils.
 */

#include <string>
#include <cstdint>
#include <lua.hpp>

#include "variant.hpp"
#include "vector.hpp"
#include "yumtable.hpp"

namespace YumEngine::utils {
  inline void set_table(lua_State *L, const YumTable &table);

  inline void push_uid(lua_State *L, const YumUID &uid) {
    lua_newtable(L);
    lua_pushstring(L, "__yumuid");
    lua_pushinteger(L, uid.bytes);
    lua_settable(L, -3);
    /**
     * note : __yumuid is a key that holds uid's bytes. It has nothing to do with __yum_type.
     */
  }

  inline void push(lua_State *L, const Variant &key) {
    switch (key.get_kind()) {
      case Variant::INTEGER: lua_pushinteger(L, key.as_int()); break;
      case Variant::NUMBER: lua_pushnumber(L, key.as_float()); break;
      case Variant::BOOLEAN: lua_pushboolean(L, key.as_bool()); break;
      case Variant::STRING: lua_pushstring(L, key.as_string().c_str()); break;
      case Variant::BINARY: {
        auto bin = key.as_binary();
        lua_pushlstring(L, (const char*)bin.start, bin.size);
      } break;
      case Variant::TABLE: {
        set_table(L, (*key.as_table())); // Variant::as_table() returns a std::shared_ptr<T>
      } break;
      case Variant::UID: push_uid(L, key.as_uid());
      case Variant::NIL: lua_pushnil(L); break; // Todo?
      default: break;
    }
  }

  inline void set_field(lua_State *L, const Variant &key, const Variant &value) {
    push(L, key);
    push(L, value);
    lua_settable(L, -3);
  }
  
  inline void set_table(lua_State *L, const YumTable &table) {
    lua_newtable(L);
    table.foreach([&L](const Variant &K, const Variant &V) {
      set_field(L, K, V);
    });
  }
  // Forward decl
  inline Variant from_lua(lua_State *L, int idx);
  
  inline YumTable get_table(lua_State *L, int index) {
    YumTable table;
    
    // Absolute index in case the stack shifts
    if (index < 0) index = lua_gettop(L) + index + 1;
    
    lua_pushnil(L); // first key
    while (lua_next(L, index) != 0) {
      
      // value at -1, key at -2
      Variant key = from_lua(L, -2);
      Variant val = from_lua(L, -1);
      
      table.set(key, val);
      
      lua_pop(L, 1); // remove value, keep key for lua_next
    }
    
    return table;
  }
 
  inline Variant from_lua(lua_State *L, int idx) {
    int type = lua_type(L, idx);

    switch (type) {
      case LUA_TNIL:
        return Variant(); // NIL
      case LUA_TBOOLEAN:
        return Variant(static_cast<bool>(lua_toboolean(L, idx)));
      case LUA_TNUMBER:
        if (lua_isinteger(L, idx))
          return Variant(static_cast<int64_t>(lua_tointeger(L, idx)));
        else
          return Variant(static_cast<double>(lua_tonumber(L, idx)));
      case LUA_TSTRING: {
        size_t len;
        const char *str = lua_tolstring(L, idx, &len);
        return Variant(std::string(str, len));
      }
      case LUA_TTABLE: {
        // Recursively convert sub-table
        YumTable sub = get_table(L, idx);

        if (sub.has_key("__yumuid")) {
          return Variant(YumUID{.bytes = sub.at("__yumuid").as_int()});
        } else if (sub.has_key("__yum_type") && sub.at("__yum_type").as_string() == "binary") {
          size_t len;
          const char *str = lua_tolstring(L, idx, &len);
          return Variant(YumBinaryBlob{.start = (const uint8_t*)str, .size = len});
        }
        return Variant(std::make_shared<YumTable>(std::move(sub)));
      }
      default:
        // For userdata, lightuserdata, threads, functions, etc.
        return Variant(); // NIL: NOT SUPPORTED BRO.
    }
  }
}