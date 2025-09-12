#include <format>
#include <chrono>
#include <memory>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "inc/subsystem.hpp"
#include "inc/variant.hpp"
#include "inc/vector.hpp"
#include "inc/glob.hpp"
#include "inc/yumdec.h"

namespace Yumcxx {
  LuaSubsystem::LuaSubsystem() { /* Might end with segfaults... */ }

  LuaSubsystem::LuaSubsystem(const std::shared_ptr<LuaCxx> &p) {
    lua = p;
  }

  LuaSubsystem::~LuaSubsystem() {/* Destructor doesn't have to do anything — LuaCxx will handle that! */}

  Variant LuaSubsystem::ati(int i) {
    auto l = lua->get();
    if (lua_isinteger(l, i)) return Variant(lua_tointeger(l, i));
    else if (lua_isnumber(l, i)) return Variant(lua_tonumber(l, i));
    else if (lua_isboolean(l, i)) return Variant((bool)lua_toboolean(l, i));
    else if (lua_isstring(l, i)) return Variant(lua_tostring(l, i));
    else return Variant(); // nil
  }

  void LuaSubsystem::push(const Variant &v) {
    if (v.is_int()) lua_pushinteger(lua->get(), v.as_int());
    else if (v.is_float()) lua_pushnumber(lua->get(), v.as_float());
    else if (v.is_bool()) lua_pushboolean(lua->get(), v.as_bool());
    else if (v.is_string()) lua_pushstring(lua->get(), v.as_string().c_str());
    else lua_pushnil(lua->get());
  }

  Vector LuaSubsystem::call(const std::string &n, const Vector &args) {
    int base = lua_gettop(lua->get());
    lua_getglobal(lua->get(), n.c_str());
    args.foreach([this](const Variant &v) { push(v); });

    if (lua_pcall(lua->get(), args.size(), LUA_MULTRET, 0) != LUA_OK) {
      ((*G_err()) << "yum: err: error during " << n << "() call: " << lua_tostring(lua->get(), -1) << std::endl);
      lua_pop(lua->get(), 1);
      return Vector();
    }

    int nresults = lua_gettop(lua->get()) - base;
    Vector ret;
    for (int i = 0; i < nresults; i++) {
      ret.append(ati(base + 1 + i));
    }

    lua_pop(lua->get(), nresults);
    return ret;
  }

  int32_t LuaSubsystem::load(const std::string &s, bool isAFileThatIHaveToLoad) {
    if (isAFileThatIHaveToLoad) {
      if (luaL_loadfile(lua->get(), s.c_str()) != LUA_OK) {
        ((*G_err()) << "yum: err: cannot load '" << s << "': " << lua_tostring(lua->get(), -1) << std::endl);
        return YumCode::YUM_ERROR;
      }
    } else {
      if (luaL_dostring(lua->get(), s.c_str()) != LUA_OK) {
        ((*G_err()) << "yum: err: cannot load <string>: " << lua_tostring(lua->get(), -1) << std::endl);
        return YumCode::YUM_ERROR;
      }
    }
    return YUM_OK;
  }

  bool LuaSubsystem::good() {
    return lua->get() != nullptr;
  }

  uint64_t Subsystem::uid_new() {
    static uint64_t counter = 0;
    return uid_base_ + (++counter);
  }

  Subsystem::Subsystem() {
    uid_base_ = (uint64_t)(std::chrono::steady_clock::now().time_since_epoch().count() / 0x1000);
  }

  Subsystem::~Subsystem() {}

  uint64_t Subsystem::newState(bool lstdlibs) {
    uint64_t uid = uid_new();

    luaSubsystems[uid] = std::make_shared<LuaSubsystem>(std::make_shared<LuaCxx>(lstdlibs));

    return uid;
  }

  void Subsystem::deleteState(uint64_t uid) {
    if (isValidUID(uid)) {
      luaSubsystems.erase(uid);
    }
  }

  bool Subsystem::isValidUID(uint64_t uid) {
    return (luaSubsystems.contains(uid));
  }

  std::shared_ptr<LuaSubsystem> Subsystem::get(uint64_t uid) {
    if (isValidUID(uid)) return luaSubsystems[uid];

    (*G_err()) << (std::format("yum: err: {}: Invalid UID", uid)) << std::endl;
    return std::make_shared<LuaSubsystem>(LuaSubsystem()); /* Be SURE that his usage will end up by segfault.
    ** And anyways, that's not MY PROBLEM. PWAHAHA, uve to use a specific API. 
    ** Then if u dont follow rules, u get segfaults. */
  }
}

extern "C" {
  // -------- Subsystem --------
  YUM_OUTATR YumSubsystem *YumSubsystem_new(void) {
    return new Yumcxx::Subsystem();
  }

  YUM_OUTATR void YumSubsystem_delete(YumSubsystem *s) {
    delete (s);
  }

  YUM_OUTATR uint64_t YumSubsystem_newState(YumSubsystem *s, int lstdlibs) {
    return (s)->newState((lstdlibs == 0));
  }

  YUM_OUTATR void YumSubsystem_deleteState(YumSubsystem *s, uint64_t uid) {
    (s)->deleteState(uid);
  }

  YUM_OUTATR int32_t YumSubsystem_isValidUID(YumSubsystem *s, uint64_t uid) {
    return (s)->isValidUID(uid);
  }

// -------- LuaSubsystem --------

  YUM_OUTATR int32_t YumLuaSubsystem_load(YumSubsystem *s, uint64_t uid, const char *src, bool isFile) {
    auto subsystem = (s);
    auto lua = subsystem->get(uid);
    return lua->load(std::string(src), isFile);
  }

  YUM_OUTATR bool YumLuaSubsystem_good(YumSubsystem *s, uint64_t uid) {
    auto subsystem = (s);
    return subsystem->get(uid)->good();
  }

  YUM_OUTATR YumVector *YumLuaSubsystem_call(YumSubsystem *s, uint64_t uid, const char *name, const YumVector *args) {
    auto subsystem = (s);
    auto lua = subsystem->get(uid);
    Yumcxx::Vector v = lua->call(std::string(name), *args);
    return new Yumcxx::Vector(std::move(v));
  }
}
