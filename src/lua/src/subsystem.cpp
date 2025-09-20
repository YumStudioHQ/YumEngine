#include <format>
#include <chrono>
#include <memory>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <unordered_map>

#include "inc/subsystem.hpp"
#include "inc/variant.hpp"
#include "inc/vector.hpp"
#include "inc/glob.hpp"
#include "inc/yumdec.h"

static inline std::vector<std::string> splitDot(const std::string& path) {
  std::vector<std::string> parts;
  size_t start = 0, dot;
  while ((dot = path.find('.', start)) != std::string::npos) {
    parts.emplace_back(path.substr(start, dot - start));
    start = dot + 1;
  }
  parts.emplace_back(path.substr(start));
  return parts;
}

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
    else if (lua_isstring(l, i)) return Variant(std::string(lua_tostring(l, i)));
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
    lua_State* L = lua->get();
    int base = lua_gettop(L);

    // So now, "Godot.Reference.CoolShit.Function()"
    // will be {"Godot", "Reference", "CoolShit", "Function()"}
    auto parts = splitDot(n);
    
    if (parts.empty()) {
      throw std::runtime_error("Invalid function name: " + n);
    }

    lua_getglobal(L, parts[0].c_str());

    for (size_t i = 1; i < parts.size(); ++i) {
      if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        throw std::runtime_error("Path " + n + " is not valid");
      }
      lua_getfield(L, -1, parts[i].c_str());
      lua_remove(L, -2);
    }

    if (!lua_isfunction(L, -1)) {
      lua_pop(L, 1);
      throw std::runtime_error("Path " + n + " is not a function");
    }

    args.foreach([this](const Variant &v) { push(v); });

    if (lua_pcall(L, args.size(), LUA_MULTRET, 0) != LUA_OK) {
      ((*G_err()) << "yum: err: error during " << n 
                  << "() call: " << lua_tostring(L, -1) << std::endl);
      lua_pop(L, 1);
      return {};
    }

    int nresults = lua_gettop(L) - base;
    Vector ret;
    for (int i = 0; i < nresults; i++) {
      ret.append(ati(base + 1 + i));
    }

    lua_pop(L, nresults);
    return ret;
  }


  int32_t LuaSubsystem::load(const std::string &s, bool isAFileThatIHaveToLoad) {
    if (isAFileThatIHaveToLoad) {
      if (luaL_dofile(lua->get(), s.c_str()) != LUA_OK) {
        ((*G_err()) << "yum: err: cannot load '" << s << "': " << lua_tostring(lua->get(), -1) << std::endl);
        return YUM_ERROR;
      }
    } else {
      if (luaL_dostring(lua->get(), s.c_str()) != LUA_OK) {
        ((*G_err()) << "yum: err: cannot load ; " << lua_tostring(lua->get(), -1) << std::endl);
        return YUM_ERROR;
      }
    }
    return YUM_OK;
  }

  bool LuaSubsystem::good() {
    return lua->get() != nullptr;
  }

  int32_t LuaSubsystem::pushCallback(
    const std::string &name,
    const std::function<Vector(Vector)> &cb,
    const std::string &ns
  ) {
    if (!cb) {
      (*G_err()) << "yum: err: pushing null callback" << std::endl;
      return YUM_ERROR;
    }
    if (name.empty()) {
      (*G_err()) << "yum: err: pushing unnamed callback (illegal)" << std::endl;
      return YUM_ERROR;
    }

    auto cb_shared = std::shared_ptr<std::function<Vector(Vector)>>(
      new std::function<Vector(Vector)>(cb),
      [name](std::function<Vector(Vector)>* p) {
        std::cout << "yum: G_sys: resource destroyed for callback: "
                  << name << "(0x" << std::hex << std::setw(16)
                  << std::setfill('0') << p << ")" << std::endl;
        delete p;
      }
    );

    // store in global map
    callbacks[(ns.empty() ? name : ns + "." + name)] = cb_shared;

    (*G_out()) << "yum: G_sys: callback pushed: "
               << (ns.empty() ? name : ns + "." + name)
               << "(0x" << std::hex << std::setw(16)
               << std::setfill('0') << cb_shared.get() << ")" << std::endl;

    lua_State* L = lua->get();

    static std::unordered_map<std::string, std::shared_ptr<std::function<Vector(Vector)>>> &callback_map = callbacks;

    auto static_lua_callback = [](lua_State *L) -> int {
      const char* cb_name = lua_tostring(L, lua_upvalueindex(1));
      if (!cb_name) return 0;

      auto it = callback_map.find(cb_name);
      if (it == callback_map.end()) return 0;

      int nargs = lua_gettop(L);
      Vector args;
      for (int i = 1; i <= nargs; ++i) {
        if (lua_isinteger(L, i)) args.append(lua_tointeger(L, i));
        else if (lua_isnumber(L, i)) args.append(lua_tonumber(L, i));
        else if (lua_isboolean(L, i)) args.append((bool)lua_toboolean(L, i));
        else if (lua_isstring(L, i)) args.append(std::string(lua_tostring(L, i)));
      }

      Vector result = (*(it->second))(args);

      for (int i = 0; i < result.size(); ++i) {
        auto me = result.at(i);
        if (me.is_int()) lua_pushinteger(L, me.as_int());
        else if (me.is_float()) lua_pushnumber(L, me.as_float());
        else if (me.is_bool()) lua_pushboolean(L, me.as_bool());
        else if (me.is_string()) lua_pushstring(L, me.as_string().c_str());
        else lua_pushnil(L);
      }

      return result.size();
    };

    // Register either globally or in namespace
    if (ns.empty()) {
      lua_pushstring(L, (ns + "." + name).c_str());
      lua_pushcclosure(L, static_lua_callback, 1);
      lua_setglobal(L, name.c_str());
    } else {
      ensureNamespace(L, ns); // leaves ns table on stack
      lua_pushstring(L, (ns + "." + name).c_str());
      lua_pushcclosure(L, static_lua_callback, 1);
      lua_setfield(L, -2, name.c_str()); // ns[name] = callback
      lua_pop(L, 1); // pop namespace
    }

    return YUM_OK;
  }

  bool LuaSubsystem::hasMethod(const std::string &path) {
    if (path.empty()) return false;

    std::istringstream iss(path);
    std::string token;
    std::vector<std::string> parts;

    while (std::getline(iss, token, '.')) {
      if (!token.empty())
        parts.push_back(token);
    }

    if (parts.empty())
      return false;

    auto L = lua->get();
    lua_getglobal(L, parts[0].c_str());

    for (size_t i = 1; i < parts.size(); i++) {
      if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return false;
      }

      lua_getfield(L, -1, parts[i].c_str());
      lua_remove(L, -2);
    }

    bool ok = lua_isfunction(L, -1);
    lua_pop(L, 1);

    return ok;
  }

  #pragma region Subsystem

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

#pragma endregion

#pragma region C Interface 

extern "C" {
  // -------- Subsystem --------
  YUM_OUTATR YumSubsystem *YumSubsystem_new(void) {
    return new Yumcxx::Subsystem();
  }

  YUM_OUTATR void YumSubsystem_delete(YumSubsystem *s) {
    delete (s);
  }

  YUM_OUTATR uint64_t YumSubsystem_newState(YumSubsystem *s, int32_t lstdlibs) {
    return (s)->newState((lstdlibs == 0));
  }

  YUM_OUTATR void YumSubsystem_deleteState(YumSubsystem *s, uint64_t uid) {
    (s)->deleteState(uid);
  }

  YUM_OUTATR int32_t YumSubsystem_isValidUID(YumSubsystem *s, uint64_t uid) {
    return (s)->isValidUID(uid);
  }

// -------- LuaSubsystem --------

  YUM_OUTATR int32_t YumLuaSubsystem_load(YumSubsystem *s, uint64_t uid, const char *src, int32_t isFile) {
    auto subsystem = (s);
    auto lua = subsystem->get(uid);
    return lua->load(std::string(src), isFile);
  }

  YUM_OUTATR int32_t YumLuaSubsystem_good(YumSubsystem *s, uint64_t uid) {
    auto subsystem = (s);
    return subsystem->get(uid)->good();
  }

  YUM_OUTATR YumVector *YumLuaSubsystem_call(YumSubsystem *s, uint64_t uid, const char *name, const YumVector *args) {
    try {
      auto subsystem = (s);
      auto lua = subsystem->get(uid);
      Yumcxx::Vector v = lua->call(std::string(name), *args);
      return new Yumcxx::Vector(std::move(v));
    } catch (const std::bad_function_call &e) {
      (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
    } catch (const std::bad_alloc &e) {
      (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
    } catch (const std::runtime_error &e) {
      (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
    } catch (const std::exception &e) {
      (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
    } catch (...) {
      (*G_err()) << "yum: G_sys: err: unknown exception caught" << std::endl;
    }

    return nullptr;
  }


  YUM_OUTATR int32_t YumLuaSubsystem_pushCallback(
    YumSubsystem *s, 
    uint64_t uid, 
    const char *name, 
    void (*cb)(const YumVector* in, YumVector* out),
    const char *ns
  ) {
    try {
      if (!s || !name || !cb) return YUM_ERROR;
      if (!s->isValidUID(uid)) return YUM_ERROR;

      auto lua = s->get(uid);

      lua->pushCallback(name, [cb](Yumcxx::Vector v) -> Yumcxx::Vector {
        Yumcxx::Vector out;
        cb(&v, &out);
        return out;
      }, /* args: const std::string &ns */ ns);

      return YUM_OK;
    } catch (const std::bad_function_call &e) {
      (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
    } catch (const std::bad_alloc &e) {
      (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
    } catch (const std::runtime_error &e) {
      (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
    } catch (const std::exception &e) {
      (*G_err()) << std::format("yum: G_sys: err: '{}' exception caught\nyum: G_sys: err: {}", typeid(e).name(), e.what()) << std::endl;
    } catch (...) {
      (*G_err()) << "yum: G_sys: err: unknown exception caught" << std::endl;
    }

    return YUM_ERROR;
  }

  YUM_OUTATR int32_t YumLuaSubsystem_hasMethod(
    YumSubsystem *s, 
    uint64_t uid, 
    const char *path) {
      if (!s || !path) return YUM_ERROR;
      if (!s->isValidUID(uid)) return YUM_ERROR;

      return (*(*s).get(uid)).hasMethod(path);
    }
}

#pragma endregion