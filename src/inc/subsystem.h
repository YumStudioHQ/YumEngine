/*********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *            This file is free & open source            *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                          from                         *
 *                         MONOE.                        *
 *                                                       *
 *********************************************************/

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

/**
 * @brief Creates a new subsystem.
 *
 * @return A pointer to a new YumSubsystem instance.
 *         Must be freed with YumSubsystem_delete().
 */
YUM_OUTATR YumSubsystem *YumSubsystem_new(void);

/**
 * @brief Deletes an existing subsystem and frees its resources.
 *
 * @param s Pointer to a YumSubsystem created by YumSubsystem_new().
 */
YUM_OUTATR void YumSubsystem_delete(YumSubsystem *s);

/**
 * @brief Creates a new Lua state within the given subsystem.
 *
 * @param s         Pointer to an existing subsystem.
 * @param lstdlibs  If nonzero, load Lua standard libraries into the state.
 *
 * @return Unique ID (UID) of the new Lua state.
 */
YUM_OUTATR uint64_t YumSubsystem_newState(YumSubsystem *s, int32_t lstdlibs);

/**
 * @brief Deletes a Lua state identified by its UID.
 *
 * @param s   Pointer to subsystem.
 * @param uid UID of the Lua state to delete.
 */
YUM_OUTATR void YumSubsystem_deleteState(YumSubsystem *s, uint64_t uid);

/**
 * @brief Checks if a UID corresponds to a valid Lua state.
 *
 * @param s   Pointer to subsystem.
 * @param uid UID to check.
 *
 * @return Nonzero (true) if valid, zero otherwise.
 */
YUM_OUTATR int32_t YumSubsystem_isValidUID(YumSubsystem *s, uint64_t uid);

/**
 * @brief Loads and executes Lua code or a Lua file.
 *
 * @param s       Pointer to subsystem.
 * @param uid     UID of the Lua state.
 * @param src     Lua code or path to file.
 * @param isFile  Nonzero if `src` is a filename; zero if it is code.
 *
 * @return YUM_OK (0) on success, YUM_ERROR on failure.
 */
YUM_OUTATR int32_t YumLuaSubsystem_load(YumSubsystem *s, uint64_t uid, const char *src, int32_t isFile);

/**
 * @brief Checks if the Lua subsystem for the given UID is valid.
 *
 * @param s   Pointer to subsystem.
 * @param uid UID of the Lua state.
 *
 * @return Nonzero (true) if Lua state is good; zero otherwise.
 */
YUM_OUTATR int32_t YumLuaSubsystem_good(YumSubsystem *s, uint64_t uid);

/**
 * @brief Calls a Lua function with arguments.
 *
 * @param s     Pointer to subsystem.
 * @param uid   UID of the Lua state.
 * @param name  Fully qualified Lua function name (e.g., "MyModule.Sub.func").
 * @param args  Pointer to YumVector containing call arguments.
 *
 * @return A pointer to a YumVector containing the return values, or NULL on error.
 *         The returned vector must be deleted by the caller.
 */
YUM_OUTATR YumVector *YumLuaSubsystem_call(YumSubsystem *s, uint64_t uid, const char *name, const YumVector *args);

/**
 * @brief Pushes a native callback into Lua.
 *
 * Registers a C function as a callable Lua function. When the Lua function
 * is called, the callback will be invoked with the given arguments and can
 * return values back to Lua.
 *
 * @param s     Pointer to subsystem.
 * @param uid   UID of the Lua state.
 * @param name  Name of the Lua function to register.
 * @param cb    Callback function pointer with signature:
 *              void cb(const YumVector *in, YumVector *out);
 * @param ns    Optional Lua namespace (use NULL for global scope).
 *
 * @return YUM_OK (0) on success, YUM_ERROR on failure.
 */
YUM_OUTATR int32_t YumLuaSubsystem_pushCallback(
  YumSubsystem *s,
  uint64_t uid,
  const char *name,
  void (*cb)(const YumVector *in, YumVector *out),
  const char *ns
);

/**
 * @brief Checks whether a Lua function exists.
 *
 * @param s     Pointer to subsystem.
 * @param uid   UID of the Lua state.
 * @param path  Fully qualified function path (e.g., "MyModule.foo").
 *
 * @return Nonzero (true) if the Lua function exists, zero otherwise.
 */
YUM_OUTATR int32_t YumLuaSubsystem_hasMethod(
  YumSubsystem *s,
  uint64_t uid,
  const char *path
);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ___YUM_SUBSYSTEM_H___