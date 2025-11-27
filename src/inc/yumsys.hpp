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
#include "yumdump.hpp"
#include "asm/dumper.hpp"

#include <chrono>
#include <thread>
#include <sstream>
#include <stdexcept>
#include <exception>
#include <algorithm>

namespace YumEngine {
  class YGlob;
  YGlob &G_yglob();

  class YGlob {
  private:
    bool api_freed = false;
    bool init_;
    pinlist pinner = pinlist (true);

    inline static std::string pretty_dumped_registers ( void ) {
      std::stringstream ss ( dump_registers() );
      std::ostringstream oss ( "" );
      std::string line;

      while ( ! ( ss.eof() ) ) {
        for (int i = 0; i < 4; i++) {
          std::string part;
          std::getline (ss, part );
          oss << part << '\t' << std::flush;
        }
        oss << std::endl;
      }

      return oss.str();
    }

    inline static std::string pretty_dumped_stacktrace ( void ) {
      std::ostringstream oss;
      oss << "stacktrace (most recent last)" << std::endl;

      auto stacktrace = get_stacktrace();
      std::reverse(stacktrace.begin(), stacktrace.end());
      for (size_t i = 0; i < stacktrace.size(); i++) {
        oss << "[" << std::dec << std::setfill('0') 
        << std::setw(2) << i << "] " << stacktrace[i] << std::endl;
      }

      return oss.str();
    } 

    inline static void dump ( void ) {
      std::stringstream ss;
      ss << dump_yum() << "\n" << pretty_dumped_registers() << pretty_dumped_stacktrace() << "\n";

      std::string line;

      while (std::getline(ss, line)) {
        (*G_out()) << line << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds ( 30 ) );
      }
    }

    inline static void yum_terminate ( void ) {
      std::exception_ptr ep = std::current_exception();
      if (ep) {
        try {
          std::rethrow_exception(ep);
        } catch (const std::exception& e) {
          (*G_err()) << "Unhandled std::exception: " << e.what() << "\n";
        } catch (...) {
          (*G_err()) << "Unhandled *non-std* exception\n";
        }
      } else {
        (*G_err()) << "terminate() called with no active exception\n";
      }

      dump();
      std::abort();
    }

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

      std::set_terminate ( yum_terminate );

      init_ = true;
    }
  };

  YGlob &G_yglob();
  extern bool is_engine_init();
}

extern "C" {
  /* C function */
  void YumEngine_init(void);
}