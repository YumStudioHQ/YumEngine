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
#ifndef ___YUM_C_TABLE_H___
#define ___YUM_C_TABLE_H___

/**
 * @file table.h
 * @brief C API for YumEngine Table — key-value container.
 *
 * Provides an opaque handle to YumEngine's table structure (`YumCTable`), 
 * which behaves similarly to a Lua table or a dictionary mapping `Variant` keys to `Variant` values.
 *
 * Use this API to interact with tables from C code.
 *
 * @warning Tables created with `YumCTable_new()` must be explicitly freed with `YumCTable_delete()`
 *          to avoid memory leaks.
 */

#include "yumdec.h"
#include "vector.h"
#include "variant.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Opaque handle representing a Yum Table object.
 *
 * Internally wraps a `YumEngine::YumTable` instance.
 */
typedef struct YumCTable YumCTable;

/** @name Lifecycle */
/** @{ */

/**
 * @brief Creates a new YumCTable instance.
 * @return Pointer to a new YumCTable object.
 *
 * @warning Must be freed with `YumCTable_delete()` when no longer needed.
 */
YUM_OUTATR YumCTable *YumCTable_new(void);

/**
 * @brief Frees a YumCTable and all its resources.
 * @param table Pointer to a YumCTable created with `YumCTable_new()`.
 */
YUM_OUTATR void YumCTable_delete(const YumCTable *table);

/** @} */

/** @name Element Access */
/** @{ */

/**
 * @brief Retrieves the value at a given key index.
 * @param table Table to query.
 * @param key A key.
 * @return Pointer to a YumVariant representing the value at the given position.
 *
 * @note The returned pointer is owned by the table — do not free it manually.
 */
YUM_OUTATR YumVariant *YumCTable_at(const YumCTable *table, const YumVariant *key);

/**
 * @brief Checks if a key exists in the table.
 * @param table Table to query.
 * @param key Pointer to a YumVariant representing the key.
 * @return Non-zero if the key exists, 0 otherwise.
 */
YUM_OUTATR int32_t YumCTable_hasKey(const YumCTable *table, const YumVariant *key);

/** @} */

/** @name Enumeration */
/** @{ */

/**
 * @brief Retrieves all keys in the table.
 * @param table Table to query.
 * @return Pointer to a YumVector containing all keys.
 *
 * @note The returned YumVector is owned by the table — do not free it manually.
 */
YUM_OUTATR YumVector *YumCTable_keys(const YumCTable *table);

/**
 * @brief Retrieves all values in the table.
 * @param table Table to query.
 * @return Pointer to a YumVector containing all values.
 *
 * @note The returned YumVector is owned by the table — do not free it manually.
 */
YUM_OUTATR YumVector *YumCTable_values(const YumCTable *table);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ___YUM_C_TABLE_H___
