/*********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *            This file is free & open source            *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                          from                         *
 *                         MONOE                         *
 *                                                       *
 *********************************************************/

#pragma once

#include <string>

namespace YumEngine::string_formats {
  inline std::string crash_header ( void ) {
    return R"( *********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *          This sofware is free & open source           *
 *                                                       *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                                                       *
 *                          from                         *
 *                         MONOE.                        *
 *                                                       *
 *  +  +            This is a crash report         +  +  *
 *                                                       *
 *********************************************************)";
  }
}
