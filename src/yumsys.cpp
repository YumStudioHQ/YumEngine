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

#include "inc/yumdec.h"
#include "inc/yumsys.hpp"

namespace YumEngine {
  YGlob &G_yglob() {
    thread_local YGlob g {};
    return g;
  }

  extern bool is_engine_init() {
    return G_yglob().is_init();
  }
}

#ifndef YUM_CXX_API_NO_C_EXPORT

extern "C" {
  YUM_OUTATR void YumEngine_init(void) {
#ifdef YUM_SHOULD_NEVER_THROW
    try {
#endif
  YumEngine::G_yglob().init();
#ifdef YUM_SHOULD_NEVER_THROW
    } catch (const std::exception &e) {
      (*G_err()) << "yum: fatal: got exception during initialization !\n\n"
                 << e.what() << std::endl;
    }
#endif
  }

  YUM_OUTATR void YUM_EXPLODE ( void ) { throw std::runtime_error ("EXPLOSION"); }
}

#endif // YUM_CXX_API_NO_C_EXPORT