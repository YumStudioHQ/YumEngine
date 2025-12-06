#include <stdint.h>

#include "inc/_byumlibc.h"
#include "inc/version/engine_version.h"

yumlibc_vdllmember uint64_t yumlibc_library_member(version_major)() {
  return YUM_ENGINE_VERSION_MAJOR;
}

yumlibc_vdllmember uint64_t yumlibc_library_member(version_minor)() {
  return YUM_ENGINE_VERSION_MINOR;
}

yumlibc_vdllmember uint64_t yumlibc_library_member(version_path)() {
  return YUM_ENGINE_VERSION_PATCH;
}

yumlibc_vdllmember uint64_t yumlibc_library_member(version)() {
  return YUM_ENGINE_VERSION_INT;
}

yumlibc_vdllmember const char* yumlibc_library_member(branch)(void) {
  return YUM_ENGINE_VERSION_BRANCH;
}

yumlibc_vdllmember const char* yumlibc_library_member(studio)(void) {
  return YUM_ENGINE_VERSION_STUDIO;
}