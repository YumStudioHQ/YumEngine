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

YUM_OUTATR YumSubsystem *YumSubsystem_new(void);
YUM_OUTATR void YumSubsystem_delete(YumSubsystem *s);
YUM_OUTATR uint64_t YumSubsystem_newState(YumSubsystem *s);
YUM_OUTATR void YumSubsystem_deleteState(YumSubsystem *s, uint64_t uid);
YUM_OUTATR int32_t YumSubsystem_isValidUID(YumSubsystem *s, uint64_t uid);
YUM_OUTATR int32_t YumLuaSubsystem_load(YumSubsystem *s, uint64_t uid, const char *src, bool isFile);
YUM_OUTATR bool YumLuaSubsystem_good(YumSubsystem *s, uint64_t uid);
YUM_OUTATR YumVector *YumLuaSubsystem_call(YumSubsystem *s, uint64_t uid, const char *name, const YumVector *args);
YUM_OUTATR int32_t YumLuaSubsystem_pushCallback(YumSubsystem *s, uint64_t uid, const char *name, YumVector (*YumCallbackFn)(YumVector));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ___YUM_SUBSYSTEM_H___