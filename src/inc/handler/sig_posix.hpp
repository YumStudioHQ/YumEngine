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

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

namespace YumEngine::os::POSIX_SYSTEMS {

  static const char* sig_to_str(int sig) {
    switch (sig) {
      case SIGSEGV: return "SIGSEGV (segmentation fault)";
      case SIGBUS:  return "SIGBUS (bus error)";
      case SIGFPE:  return "SIGFPE (floating point exception)";
      case SIGILL:  return "SIGILL (illegal instruction)";
      case SIGABRT: return "SIGABRT (abort)";
      default:      return "UNKNOWN SIGNAL";
    }
  }

  inline void safe_write(const char* s) {
    write(STDERR_FILENO, s, strlen(s));
  }

  inline void handler(int sig, siginfo_t* info, void* uctx_raw) {
    safe_write("\n======================================================\n");
    safe_write("    YumEngine Crash Signal Handler (POSIX)\n");
    safe_write("======================================================\n");

    safe_write("Signal: ");
    safe_write(sig_to_str(sig));
    safe_write("\n");

    // Fault address if any
    if (info) {
      char buf[64];
      uintptr_t addr = (uintptr_t)info->si_addr;
      int len = snprintf(buf, sizeof(buf),
         "Fault address: 0x%lx\n", (unsigned long)addr);
      write(STDERR_FILENO, buf, len);
    }

    ucontext_t* uctx = (ucontext_t*)uctx_raw;


    safe_write("Raw register dump complete (safe mode).\n");

    safe_write("\n!!! CRASH OCCURRED !!!\n");
    safe_write("Exiting now.\n\n");

    _exit(1); // async-signal-safe hard exit
  }

  inline void setupSignals() {
    struct sigaction sa {};
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGBUS,  &sa, nullptr);
    sigaction(SIGFPE,  &sa, nullptr);
    sigaction(SIGILL,  &sa, nullptr);
    sigaction(SIGABRT, &sa, nullptr);
  }
} // namespace
