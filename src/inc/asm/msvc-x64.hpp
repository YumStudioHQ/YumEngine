#pragma once

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

#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <intrin.h>

namespace YumEngine {
  inline std::string dump_registers() {
    uint64_t rax = __readrax();
    uint64_t rbx = __readrbx();
    uint64_t rcx = __readrcx();
    uint64_t rdx = __readrdx();
    uint64_t rsi = __readrsi();
    uint64_t rdi = __readrdi();
    uint64_t rbp = __readrbp();
    uint64_t rsp = __readrsp();
    uint64_t r8  = __readr8();
    uint64_t r9  = __readr9();
    uint64_t r10 = __readr10();
    uint64_t r11 = __readr11();
    uint64_t r12 = __readr12();
    uint64_t r13 = __readr13();
    uint64_t r14 = __readr14();
    uint64_t r15 = __readr15();

    std::ostringstream o;
    o << std::hex << std::setfill('0');

#define P(R) o << #R " = 0x" << std::setw(16) << R << "\n"
    P(rax); P(rbx); P(rcx); P(rdx);
    P(rsi); P(rdi); P(rbp); P(rsp);
    P(r8);  P(r9);  P(r10); P(r11);
    P(r12); P(r13); P(r14); P(r15);
#undef P

    return o.str();
  }
}