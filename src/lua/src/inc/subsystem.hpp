#pragma once

/* Subsystem — set of classes that helps you to manage Lua code */

#include <memory>
#include <cstdint>
#include <iostream>
#include <unordered_map>

#include "lua.hpp"

#include "luacpp.hpp"
#include "vector.hpp"

namespace Yumcxx {
  class LuaSubsystem {
  private:
    std::shared_ptr<LuaCxx> lua;
  
    void push(const Variant&);
    Variant ati(int);

  public:
    LuaSubsystem();
    LuaSubsystem(const std::shared_ptr<LuaCxx>&);
    ~LuaSubsystem();

    Vector call(const std::string&, const Vector&);
    int32_t load(const std::string&, bool); /* true: file, false: string */
    bool good();
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