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

/**
 * @brief brings a global type interface for global API-system management.
*/

#include "pinlist.hpp"

#include <stdexcept>


namespace YumEngine {
  class YGlob;
  YGlob &G_yglob();

  class YGlob {
  private:
    bool api_freed = false;
    bool init_;
    pinlist pinner;

  public:
    inline void free() {
      if (api_freed) return;
      pinner.finalize();
      api_freed = true;
    }

    inline pinlist &get() { return pinner; }
    inline bool is_init() { return init_; }

    inline void init() {
      if ( (std::atexit([]() { G_yglob().free(); })) ) {
        throw std::runtime_error("Unable to push cleaner in atexit(), cannot run.");
      }

      if ( (std::at_quick_exit([]() { G_yglob().free(); })) ) {
        throw std::runtime_error("Unable to push cleaner in at_quick_exit(), cannot run.");
      }
      init_ = true;
    }
  };

  YGlob &G_yglob();
  extern bool is_engine_init();
}