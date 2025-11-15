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

namespace YumEngine {
  inline std::string dump_registers() {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;

    asm volatile(
      "mov %%rax, %0\n\t"   "mov %%rbx, %1\n\t"
      "mov %%rcx, %2\n\t"   "mov %%rdx, %3\n\t"
      "mov %%rsi, %4\n\t"   "mov %%rdi, %5\n\t"
      "mov %%rbp, %6\n\t"   "mov %%rsp, %7\n\t"
      "mov %%r8,  %8\n\t"   "mov %%r9,  %9\n\t"
      "mov %%r10, %10\n\t"  "mov %%r11, %11\n\t"
      "mov %%r12, %12\n\t"  "mov %%r13, %13\n\t"
      "mov %%r14, %14\n\t"  "mov %%r15, %15\n\t"
      : "=m"(rax), "=m"(rbx), "=m"(rcx), "=m"(rdx),
        "=m"(rsi), "=m"(rdi), "=m"(rbp), "=m"(rsp),
        "=m"(r8), "=m"(r9), "=m"(r10), "=m"(r11),
        "=m"(r12), "=m"(r13), "=m"(r14), "=m"(r15)
    );

    std::ostringstream o;
    o << std::hex << std::setfill('0');

#define P(reg) o << #reg " = 0x" << std::setw(16) << reg << "\n"
    P(rax); P(rbx); P(rcx); P(rdx);
    P(rsi); P(rdi); P(rbp); P(rsp);
    P(r8);  P(r9);  P(r10); P(r11);
    P(r12); P(r13); P(r14); P(r15);
#undef P

    return o.str();
  }
}