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

#if defined(__clang__)
# define COMPILER_CLANG 1
#elif defined(__GNUC__)
# define COMPILER_GCC 1
#elif defined(_MSC_VER)
# define COMPILER_MSVC 1
#else
# warning "Compiling with unknown compiler"
#endif

#if defined(__i386__) || defined(_M_IX86)
# define ARCH_X86 1
#elif defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
# define ARCH_X64 1
#elif defined(__arm__) || defined(_M_ARM)
# define ARCH_ARM32 1
#elif defined(__aarch64__) || defined(_M_ARM64)
# define ARCH_ARM64 1
#else
# warning "Unknown architecture"
#endif

#if COMPILER_GCC || COMPILER_CLANG
# define COMPILER_GCC_FAMILY 1
#endif

#if ARCH_X86
# if COMPILER_GCC_FAMILY
#  include "gcc-x86.hpp"
# else
#  include "msvc-x86.hpp"
# endif
#elif ARCH_X64
# if COMPILER_GCC_FAMILY
#  include "gcc-x64.hpp"
# else
#  include "msvc-x64.hpp"
# endif
#elif ARCH_ARM32
# if COMPILER_GCC_FAMILY
#  include "gcc-armv7.hpp"
# else
   namespace YumEngine {inline std::string dump_registers() { return "register dump not avaible"; }}
#  ifdef YUM_PROMOTES_WARNINGS
#   error "No such helper function for dumping registers on this platform."
#  elif YUM_MUTTERS_WARNINGS
#  else
#   warning "No such helper function for dumping registers on this platform."
#  endif
# endif
#elif ARCH_ARM64
# if COMPILER_GCC_FAMILY
#  include "gcc-arm64.hpp"
# else
   namespace YumEngine {inline std::string dump_registers() { return "register dump not avaible"; }}
#  ifdef YUM_PROMOTES_WARNINGS
#   error "No such helper function for dumping registers on this platform."
#  elif YUM_MUTTERS_WARNINGS
#  else
#   warning "No such helper function for dumping registers on this platform."
#  endif
# endif
#endif