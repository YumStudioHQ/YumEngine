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
    uint32_t eax, ebx, ecx, edx, esi, edi, ebp, esp;
    asm volatile(
      "movl %%eax, %0\n\t"
      "movl %%ebx, %1\n\t"
      "movl %%ecx, %2\n\t"
      "movl %%edx, %3\n\t"
      "movl %%esi, %4\n\t"
      "movl %%edi, %5\n\t"
      "movl %%ebp, %6\n\t"
      "movl %%esp, %7\n\t"
      : "=g"(eax), "=g"(ebx), "=g"(ecx), "=g"(edx),
        "=g"(esi), "=g"(edi), "=g"(ebp), "=g"(esp)
    );

    std::ostringstream out;
    out << std::hex << std::setfill('0');
    out << "EAX = 0x" << std::setw(8) << eax << "\n";
    out << "EBX = 0x" << std::setw(8) << ebx << "\n";
    out << "ECX = 0x" << std::setw(8) << ecx << "\n";
    out << "EDX = 0x" << std::setw(8) << edx << "\n";
    out << "ESI = 0x" << std::setw(8) << esi << "\n";
    out << "EDI = 0x" << std::setw(8) << edi << "\n";
    out << "EBP = 0x" << std::setw(8) << ebp << "\n";
    out << "ESP = 0x" << std::setw(8) << esp << "\n";
    return out.str();
  }
}