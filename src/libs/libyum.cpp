#include <tuple>
#include <string>
#include <chrono>
#include <format>
#include <thread>
#include <cstdint>
#include <iostream>
#include <filesystem>

#include <lua.hpp>

#include "libyumv.h"

#include "../lua/yumexport.h"

namespace Yum {
  namespace Yum_cxxlibs {
    namespace LuaHelpers {

      // -----------------------------
      // Arg getters
      // -----------------------------
      template<typename T>
      T get_arg(lua_State* L, int index);

      template<>
      int64_t get_arg<int64_t>(lua_State* L, int index) {
        return static_cast<int64_t>(luaL_checkinteger(L, index));
      }

      template<>
      double get_arg<double>(lua_State* L, int index) {
        return luaL_checknumber(L, index);
      }

      template<>
      bool get_arg<bool>(lua_State* L, int index) {
        return lua_toboolean(L, index) != 0;
      }

      template<>
      std::string get_arg<std::string>(lua_State* L, int index) {
        return luaL_checkstring(L, index);
      }

      // -----------------------------
      // Push single return
      // -----------------------------
      inline void push(lua_State* L, int64_t val) { lua_pushinteger(L, val); }
      inline void push(lua_State* L, double val) { lua_pushnumber(L, val); }
      inline void push(lua_State* L, bool val) { lua_pushboolean(L, val); }
      inline void push(lua_State* L, const std::string& val) { lua_pushstring(L, val.c_str()); }

      // -----------------------------
      // Wrapper
      // -----------------------------
      template<typename Ret, typename... Args>
      int call(lua_State* L, Ret(*func)(Args...)) {
        std::tuple<Args...> args;
        int idx = 1;
        ((std::get<Args>(args) = get_arg<Args>(L, idx++)), ...);
        Ret ret = std::apply(func, args);
        push(L, ret);
        return 1;
      }

    } /* LuaHelpers */

    class Yum {

    public:
      Yum(lua_State* L, const std::string& tableName) :
        L(L), tableName(tableName) {
        lua_newtable(L);
        lua_setglobal(L, tableName.c_str());
      }

      Yum inner(const std::string& name) {
        lua_getglobal(L, tableName.c_str());
        lua_getfield(L, -1, name.c_str());
        if (lua_isnil(L, -1)) {
          lua_pop(L, 1);
          lua_newtable(L);
          lua_setfield(L, -2, name.c_str());
          lua_getfield(L, -1, name.c_str());
        }
        lua_remove(L, -2);
        return Yum(L, name);
      }

      template<typename Ret, typename... Args>
      void func(const std::string& name, Ret(*f)(Args...)) {
        lua_getglobal(L, tableName.c_str());
        /* Store pointer in Lua userdata so lambda can capture */
        Ret(**ptr)(Args...) = (Ret(**)(Args...))lua_newuserdata(L, sizeof(f));
        *ptr = f;

        lua_pushcclosure(L, [](lua_State* L) -> int {
          Ret(**fp)(Args...) = (Ret(**)(Args...))lua_touserdata(L, lua_upvalueindex(1));
          return LuaHelpers::call(L, *fp);
        }, 1);

        lua_setfield(L, -2, name.c_str());
        lua_pop(L, 1);
      }

    private:
      lua_State* L;
      std::string tableName;

    };
  }
}

std::string libyum_version() {
  return std::format("YumEngine.libyum.{}.{}.{}.{}.{}", 
      LIBYUM_STUDIO, LIBYUM_BRANCH, LIBYUM_VERSION_MAJOR, LIBYUM_VERSION_MINOR, LIBYUM_VERSION_PATCH
    );
}

int64_t libyum_iversion() {
  return LIBYUM_VERSION;
}

YUM_API void libyum_openlib(lua_State *L) {
  std::cout << libyum_version() << std::endl;

  Yum::Yum_cxxlibs::Yum Y(L, "Yum");

  auto fs = Y.inner("fs");
  auto sys = Y.inner("sys");
  // auto godot = Y.inner("godot"); // TODO ! LATER
  // auto http = Y.inner("http");
  auto vfs = Y.inner("vfs");

  Y.func("version", libyum_version);
  Y.func("iversion", libyum_iversion);
}