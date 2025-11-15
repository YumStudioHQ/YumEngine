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

 #pragma once

/**
 * @file subsystem.hpp
 * @brief Lua Subsystem management — integrates YumEngine's Lua runtime with native code.
 *
 * This header defines the core classes responsible for managing Lua states and
 * the binding of native callbacks to the Lua environment.  
 * It provides two main components:
 * - `LuaSubsystem`: manages a single Lua state and registered native callbacks.
 * - `Subsystem`: manages multiple LuaSubsystem instances, identified by unique IDs.
 */

#include <string>
#include <memory>
#include <cstdint>
#include <iostream>
#include <functional>
#include <unordered_map>

#include "lua.hpp"
#include "luacpp.hpp"
#include "vector.hpp"

#include "pinlist.hpp"

namespace YumEngine {

  /**
   * @typedef ManagedCallback
   * @brief Function type representing a managed callback.
   *
   * Used to represent native callbacks callable from Lua.
   * A callback receives a `Vector` of arguments and returns a `Vector` of results.
   */
  using ManagedCallback = std::function<Vector(Vector)>;

  /**
   * @class LuaSubsystem
   * @brief Represents a single Lua execution context and its native callback bindings.
   *
   * `LuaSubsystem` encapsulates a Lua state (`LuaCxx`), along with a set of registered
   * native callbacks. It provides mechanisms for:
   * - Registering and invoking native callbacks from Lua.
   * - Loading and executing Lua code (either from file or string).
   * - Maintaining and cleaning up Lua-managed resources.
   *
   * @note Callbacks are owned by the language runtime that registered them.
   *       The backend only manages the lifetime of their wrapper, not the callback itself.
   */
  class LuaSubsystem {
  private:
    /** @brief Registered native callbacks mapped by name. */
    std::unordered_map<std::string, std::shared_ptr<ManagedCallback>> callbacks;

    /** @brief Associated Lua interpreter instance. */
    std::shared_ptr<LuaCxx> lua;

    /** @brief Pushes a Variant value onto the Lua stack. */
    void pushOnLuaStack(const Variant &);

    /** @brief Garbage Collector (can free unfreed resources when destroyed correctly) */
    mutable pinlist pinlist_;

  public:
    /** @name Constructors and Destructor */
    /** @{ */

    /** @brief Creates a new LuaSubsystem with a fresh Lua state. */
    LuaSubsystem();

    /**
     * @brief Creates a LuaSubsystem using an existing LuaCxx instance.
     * @param lua Shared pointer to an existing LuaCxx context.
     */
    LuaSubsystem(const std::shared_ptr<LuaCxx> &lua);

    /** @brief Destroys the LuaSubsystem and cleans up resources. */
    ~LuaSubsystem();

    /** @} */

    /** @name Execution and Loading */
    /** @{ */

    /**
     * @brief Calls a Lua function by name.
     * @param name Name of the Lua function.
     * @param args Arguments to pass to the function.
     * @return A `Vector` containing the function’s return values.
     */
    Vector call(const std::string &name, const Vector &args);

    /**
     * @brief Loads Lua source code.
     * @param source File path or source string.
     * @param isFile If `true`, loads from file; if `false`, treats `source` as raw Lua code.
     * @return Zero on success, or a non-zero error code.
     */
    int32_t load(const std::string &source, bool isFile);

    /**
     * @brief Checks whether the underlying Lua state is valid.
     * @return `true` if the Lua state is valid; otherwise `false`.
     */
    bool good();

    /** @} */

    /** @name Callback Management */
    /** @{ */

    /**
     * @brief Registers a new native callback callable from Lua.
     * @param name Name under which the callback will be exposed to Lua.
     * @param cb Function to be registered.
     * @param ns Optional Lua namespace to register under (defaults to global scope).
     * @return Zero on success, or a non-zero error code.
     */
    int32_t pushCallback(const std::string &name, const ManagedCallback &cb, const std::string &ns = "");

    /**
     * @brief Registers a managed callback using a shared pointer.
     * @param name Name under which the callback will be exposed to Lua.
     * @param cb Shared pointer to the callback function.
     * @param ns Optional Lua namespace to register under (defaults to global scope).
     * @return Zero on success, or a non-zero error code.
     */
    int32_t pushCallback(const std::string &name, const std::shared_ptr<ManagedCallback> &cb, const std::string &ns = "");

    /**
     * @brief Checks whether a Lua method is defined.
     * @param name Name of the Lua function.
     * @return `true` if the method exists, otherwise `false`.
     */
    bool hasMethod(const std::string &name);

    /** @} */

    /** @name Utility */
    /** @{ */

    /**
     * @brief Ensures that a Lua namespace exists in the global environment.
     * @param L Pointer to the Lua state.
     * @param ns Namespace name.
     *
     * If the namespace table does not exist, it is created.
     */
    inline static void ensureNamespace(lua_State *L, const std::string &ns) {
      lua_getglobal(L, ns.c_str());  // push ns onto stack
      if (lua_isnil(L, -1)) {
        lua_pop(L, 1);               // remove nil
        lua_newtable(L);             // create new table
        lua_setglobal(L, ns.c_str());
        lua_getglobal(L, ns.c_str()); // push it again
      }
      // stack now has namespace table
    }

    /** @brief returns the pinlist. */
    inline pinlist *get_pinlist() const { return &pinlist_; }

    /** @} */
  };

  /**
   * @class Subsystem
   * @brief UID-based manager for multiple LuaSubsystem instances.
   *
   * The `Subsystem` class acts as a container that manages multiple Lua states,
   * each represented by a `LuaSubsystem` object and identified by a unique 64-bit ID (UID).
   * 
   * It is responsible for:
   * - Creating and deleting Lua states.
   * - Ensuring unique identifiers for each instance.
   * - Providing access to existing LuaSubsystems.
   *
   * Uses `std::shared_ptr` for automatic memory management and safe sharing.
   */
  class Subsystem {
  private:
    /** @brief Current base value for UID generation. */
    uint64_t uid_base_;

    /** @brief Generates a new unique identifier. */
    uint64_t uid_new();

    /** @brief Map of active LuaSubsystem instances indexed by UID. */
    std::unordered_map<uint64_t, std::shared_ptr<LuaSubsystem>> luaSubsystems;

  public:
    /** @name Constructors and Destructor */
    /** @{ */

    /** @brief Creates an empty Subsystem manager. */
    Subsystem();

    /** @brief Destroys the Subsystem and all associated Lua states. */
    ~Subsystem();

    /** @} */

    /** @name Lua State Management */
    /** @{ */

    /**
     * @brief Creates a new Lua state and assigns it a unique ID.
     * @param withLibs If `true`, loads standard Lua libraries; otherwise creates a bare state.
     * @return The unique ID assigned to the new state.
     */
    uint64_t newState(bool withLibs = false);

    /**
     * @brief Deletes a Lua state by its unique ID.
     * @param uid Unique ID of the Lua state to delete.
     */
    void deleteState(uint64_t uid);

    /**
     * @brief Checks whether a given UID corresponds to a valid LuaSubsystem.
     * @param uid Unique ID to check.
     * @return `true` if the UID is valid, otherwise `false`.
     */
    bool isValidUID(uint64_t uid);

    /**
     * @brief Retrieves a LuaSubsystem by its unique ID.
     * @param uid Unique ID of the desired LuaSubsystem.
     * @return Shared pointer to the LuaSubsystem, or `nullptr` if invalid.
     */
    std::shared_ptr<LuaSubsystem> get(uint64_t uid);

    /** @} */
  };

} // namespace YumEngine

/**
 * @typedef YumSubsystem
 * @brief Alias for `YumEngine::Subsystem` (C-API compatible).
 */
using YumSubsystem = YumEngine::Subsystem;

/**
 * @typedef YumLuaSubsystem
 * @brief Alias for `YumEngine::LuaSubsystem` (C-API compatible).
 */
using YumLuaSubsystem = YumEngine::LuaSubsystem;
