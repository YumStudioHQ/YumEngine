#pragma once
#include <string>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

#include "action.hpp"

namespace YumEngine {
  inline std::string GetEnv(const std::string& key) {
  #ifdef _WIN32
    char buffer[32767]; // Windows max env var length
    DWORD len = GetEnvironmentVariableA(key.c_str(), buffer, sizeof(buffer));
    if (len == 0) return ""; // not found
    return std::string(buffer, len);
  #else
    const char* val = std::getenv(key.c_str());
    return val ? std::string(val) : "";
  #endif
  }

  inline void SetEnv(const std::string& key, const std::string& value) {
  #ifdef _WIN32
    if (!SetEnvironmentVariableA(key.c_str(), value.c_str())) {
      throw std::runtime_error("Failed to set environment variable: " + key);
    }
  #else
    if (setenv(key.c_str(), value.c_str(), 1) != 0) {
      throw std::runtime_error("Failed to set environment variable: " + key);
    }
  #endif
  }

  inline void UnsetEnv(const std::string& key) {
  #ifdef _WIN32
    if (!SetEnvironmentVariableA(key.c_str(), nullptr)) {
      throw std::runtime_error("Failed to unset environment variable: " + key);
    }
  #else
    if (unsetenv(key.c_str()) != 0) {
      throw std::runtime_error("Failed to unset environment variable: " + key);
    }
  #endif
  }

  namespace Actors {

    inline Action<void, std::string> GetEnvActor(const std::string &key) {
      return Action<void, std::string>([key](void) {
        return (GetEnv(key));
      });
    }

    inline Action<void, std::string> EnsureEnvActor(const std::string &key, const std::string &deflt = "") {
      return Action<void, std::string>([key, deflt](void) {
        auto e = (GetEnv(key));
        if (e.empty()) SetEnv(key, deflt);
        return e;
      });
    }
  }
}
