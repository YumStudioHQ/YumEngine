#pragma once

/* Subsystem — set of classes that helps you to manage Lua code */

#include <string>
#include <memory>
#include <cstdint>
#include <iostream>
#include <functional>
#include <unordered_map>

#include "lua.hpp"

#include "luacpp.hpp"
#include "vector.hpp"

namespace Yumcxx {
  using ManagedCallback = std::function<Vector(Vector)>;

  class LuaSubsystem {
  private:
    std::unordered_map<std::string, std::shared_ptr<ManagedCallback>> callbacks;
    std::shared_ptr<LuaCxx> lua;
  
    void pushOnLuaStack(const Variant&);
    Variant ati(int);

  public:
    LuaSubsystem();
    LuaSubsystem(const std::shared_ptr<LuaCxx>&);
    ~LuaSubsystem();

    Vector call(const std::string&, const Vector&);
    int32_t load(const std::string&, bool); /* true: file, false: string */
    bool good();
    int32_t pushCallback(const std::string&, const ManagedCallback&, const std::string& = "");
    int32_t pushCallback(const std::string&, const std::shared_ptr<ManagedCallback>&, const std::string& = "");
    bool hasMethod(const std::string&);

    inline static void ensureNamespace(lua_State* L, const std::string& ns) {
      lua_getglobal(L, ns.c_str());  // push ns onto stack
      if (lua_isnil(L, -1)) {
        lua_pop(L, 1);             // remove nil
        lua_newtable(L);           // create new table
        lua_setglobal(L, ns.c_str());
        lua_getglobal(L, ns.c_str()); // push it again
      }
      // stack now has namespace table
    }
  };

  class Subsystem {
  private:
    uint64_t uid_base_;
    uint64_t uid_new();
    
    std::unordered_map<uint64_t, std::shared_ptr<LuaSubsystem>> luaSubsystems;

  public:
    Subsystem();
    ~Subsystem();

    uint64_t newState(bool = false);
    void deleteState(uint64_t);
    bool isValidUID(uint64_t);
    std::shared_ptr<LuaSubsystem> get(uint64_t);
  };
}

using YumSubsystem = Yumcxx::Subsystem;
using YumLuaSubsystem = Yumcxx::LuaSubsystem;