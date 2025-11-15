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

#include "engine_version.h"
#include "format/crash_header.hpp"

#include <string>
#include <sstream>

namespace YumEngine {
  inline std::string dump_yum ( void ) {
    std::stringstream ss;

    ss << string_formats::crash_header() + '\n'
       << "major: " << YUM_ENGINE_VERSION_MAJOR << '\t'
       << "minor: " << YUM_ENGINE_VERSION_MINOR << '\t'
       << "patch: " << YUM_ENGINE_VERSION_PATCH << '\n'
       << "branch: " << YUM_ENGINE_VERSION_BRANCH << '\n'
       << "studio: " << YUM_ENGINE_VERSION_STUDIO << '\n';

    return ss.str();
  }
}