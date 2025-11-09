#ifndef ___YUM_SUBSYSTEM_H___
#define ___YUM_SUBSYSTEM_H___
#include <stdint.h>
#include "vector.h"
#include "yumdec.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct YumSubsystem YumSubsystem;
typedef struct YumLuaSubsystem YumLuaSubsystem;

YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR YumSubsystem *YumSubsystem_new(void);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR void          YumSubsystem_delete(YumSubsystem *s);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR uint64_t      YumSubsystem_newState(YumSubsystem *s);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR void          YumSubsystem_deleteState(YumSubsystem *s, uint64_t uid);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR int32_t       YumSubsystem_isValidUID(YumSubsystem *s, uint64_t uid);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR int32_t       YumLuaSubsystem_load(YumSubsystem *s, uint64_t uid, const char *src, bool isFile);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR int32_t       YumLuaSubsystem_good(YumSubsystem *s, uint64_t uid);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR YumVector    *YumLuaSubsystem_call(YumSubsystem *s, uint64_t uid, const char *name, const YumVector *args);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR int32_t       YumLuaSubsystem_pushCallback(
  YumSubsystem *s, 
  uint64_t uid, 
  const char *name, 
  void (*cb)(const YumVector* in, YumVector* out),
  const char *ns
);

YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR int32_t YumLuaSubsystem_hasMethod(
  YumSubsystem *s, 
  uint64_t uid, 
  const char *path
);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ___YUM_SUBSYSTEM_H___