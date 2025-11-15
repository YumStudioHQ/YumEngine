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

#include <string>
#include <format>

#include "inc/yumdec.h"
#include "inc/engine_version.h"

extern "C" {
  /* Promised in all version since 1.6.X */
  YUM_OUTATR const char *YumEngineInfo_name() {
    static std::string name = std::format(
      "YumEngineCAPI — {}.{}.YumEngine.{}.{}.{}",
      YUM_ENGINE_VERSION_STUDIO, YUM_ENGINE_VERSION_BRANCH, 
      YUM_ENGINE_VERSION_MAJOR, YUM_ENGINE_VERSION_MINOR, YUM_ENGINE_VERSION_PATCH
    );
    return name.c_str();
  }

  /* Promised in all version since 1.6.X */
  YUM_OUTATR const char *YumEngineInfo_studioName() { return YUM_ENGINE_VERSION_STUDIO; }
  /* Promised in all version since 1.6.X */
  YUM_OUTATR const char *YumEngineInfo_branch() { return YUM_ENGINE_VERSION_BRANCH; }
  /* Promised in all version since 1.6.X */
  YUM_OUTATR int32_t YumEngineInfo_versionMajor() { return YUM_ENGINE_VERSION_MAJOR;  }
  /* Promised in all version since 1.6.X */
  YUM_OUTATR int32_t YumEngineInfo_versionMinor() { return YUM_ENGINE_VERSION_MINOR;  }
  /* Promised in all version since 1.6.X */
  YUM_OUTATR int32_t YumEngineInfo_versionPatch() { return YUM_ENGINE_VERSION_PATCH;  }
  /* Promised in all version since 1.6.X */
  YUM_OUTATR int64_t YumEngineInfo_longVersion() { return YUM_ENGINE_VERSION_INT; }
}