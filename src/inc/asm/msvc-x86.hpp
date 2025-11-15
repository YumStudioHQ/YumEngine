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
    uint32_t r[16]; // r0-r15
    asm volatile("stmia %0, {r0-r15}" :: "r"(r));

    std::ostringstream o;
    o << std::hex << std::setfill('0');

    for (int i = 0; i <= 12; i++)
        o << "R" << std::dec << std::setw(2) 
        << i << "  = 0x" << std::hex << std::setw(8) << r[i] << "\n";

    o << "SP = 0x" << std::setw(8) << r[13] << "\n";
    o << "LR = 0x" << std::setw(8) << r[14] << "\n";
    o << "PC = 0x" << std::setw(8) << r[15] << "\n";

    return o.str();
  }
}