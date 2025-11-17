#include <stdio.h>
#include <stdint.h>

#include "inc/yum.h"

int main ( void ) {
  YumEngine_init();

  YumSubsystem *subsystem = YumSubsystem_new();

  uint64_t uid = YumSubsystem_newState(subsystem, 1); // 1 -> Lua libraries, 0 -> no library

  if (!YumSubsystem_isValidUID(subsystem, uid)) {
    perror("Sorry ! I got invalid UID, I cannot follow this tutorial anymore ...");
    return YUM_ERROR;
  }

  const char *code = "print('Hello, world, from Lua, loaded in C haha !')";
  YumLuaSubsystem_load(subsystem, uid, code, 0);

  YumSubsystem_delete(subsystem);

  return 0;
}