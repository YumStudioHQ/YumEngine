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
    uint64_t x[32];

    asm volatile(
      "stp x0,  x1,  [%0, #0]\n\t"
      "stp x2,  x3,  [%0, #16]\n\t"
      "stp x4,  x5,  [%0, #32]\n\t"
      "stp x6,  x7,  [%0, #48]\n\t"
      "stp x8,  x9,  [%0, #64]\n\t"
      "stp x10, x11, [%0, #80]\n\t"
      "stp x12, x13, [%0, #96]\n\t"
      "stp x14, x15, [%0, #112]\n\t"
      "stp x16, x17, [%0, #128]\n\t"
      "stp x18, x19, [%0, #144]\n\t"
      "stp x20, x21, [%0, #160]\n\t"
      "stp x22, x23, [%0, #176]\n\t"
      "stp x24, x25, [%0, #192]\n\t"
      "stp x26, x27, [%0, #208]\n\t"
      "stp x28, x29, [%0, #224]\n\t"
      "str x30, [%0, #240]\n\t"
      "mov x1, sp\n\t"
      "str x1, [%0, #248]\n\t"
      :: "r"(x) : "memory"
    );

    std::ostringstream o;
    o << std::hex << std::setfill('0');

    for (int i = 0; i <= 30; i++)
        o << "X" << std::dec << std::setw(2) << i << " = 0x" 
        << std::hex << std::setw(16) << x[i] << "\n";
    o << "SP  = 0x" << std::setw(16) << x[31] << "\n";

    return o.str();
  }
}