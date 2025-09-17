#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <functional>
#include <unordered_map>

#include "../lua/src/inc/subsystem.hpp"

namespace Yumcxx {
  class YumRuntimeElement {
  private:
    int64_t uid_base_;

  protected:
    inline int64_t uid_next() { return uid_base_++; }

    std::string ns;
    std::unordered_map<std::string, std::shared_ptr<ManagedCallback>> table;
  public:
    inline YumRuntimeElement()
      : uid_base_((int64_t)(std::chrono::steady_clock::now().time_since_epoch().count())) {}

    inline void open(const std::function<int32_t(
      const std::string&, 
      const std::shared_ptr<ManagedCallback>&, 
      const std::string&)> 
    &callback) {
      for (const auto &[name, cb] : table) {
        callback(name, cb, ns);
      }
    }
  };

  class YumRuntimeLibrary {
  private:
    std::unordered_map<uint64_t, std::shared_ptr<std::iostream>> streams;
    std::vector<YumRuntimeElement> runtimeElements;

  public:
    YumRuntimeLibrary() {}
    YumRuntimeLibrary(const std::vector<YumRuntimeElement> &vec) 
    : runtimeElements(vec) {}

    inline void OpenLibrary(LuaSubsystem *sub) {
      for (auto &rtl : runtimeElements) {
        rtl.open([&sub](const std::string &name, 
        const std::shared_ptr<ManagedCallback> &callback, 
        const std::string &ns) -> int32_t {
          return (*sub).pushCallback(name, callback, ns);
        });
      }
    }
  };
}