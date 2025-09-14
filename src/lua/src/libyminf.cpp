#include <string>
#include <format>

#include "inc/yumdec.h"
#include "inc/engine_version.h"

extern "C" {
  YUM_OUTATR const char *YumEngineInfo_name() {
    static std::string name = std::format(
      "YumEngineCAPI — {}.{}.YumEngine.{}.{}.{}",
      YUM_ENGINE_VERSION_STUDIO, YUM_ENGINE_VERSION_BRANCH, 
      YUM_ENGINE_VERSION_MAJOR, YUM_ENGINE_VERSION_MINOR, YUM_ENGINE_VERSION_PATCH
    );
    return name.c_str();
  }

  YUM_OUTATR const char *YumEngineInfo_studioName() { return YUM_ENGINE_VERSION_STUDIO; }
  YUM_OUTATR const char *YumEngineInfo_branch()     { return YUM_ENGINE_VERSION_BRANCH; }
  YUM_OUTATR int YumEngineInfo_versionMajor()       { return YUM_ENGINE_VERSION_MAJOR;  }
  YUM_OUTATR int YumEngineInfo_versionMinor()       { return YUM_ENGINE_VERSION_MINOR;  }
  YUM_OUTATR int YumEngineInfo_versionPatch()       { return YUM_ENGINE_VERSION_PATCH;  }
}