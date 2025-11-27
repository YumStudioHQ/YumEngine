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

#ifndef ___YUM_VECTOR_H___
#define ___YUM_VECTOR_H___

/**
 * @file vector.h
 * @brief C API for Yum Vector — dynamic array of Variants.
 *
 * YumVector is a dynamic list-like container holding YumVariant elements.
 * It provides simple creation, destruction, append, access, and size operations.
 *
 * Vectors are used to represent Lua argument lists and return values
 * when interacting through the Yum C API.
 */

#include <stdint.h>
#include "yumdec.h"
#include "variant.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef YUM_CXX_API_NO_C_EXPORT

/**
 * @brief Opaque handle to a Yum Vector.
 */
typedef struct YumVector YumVector;

/**
 * @brief Creates a new empty vector.
 *
 * @return Pointer to a new YumVector instance.
 *         Must be deleted with YumVector_delete().
 */

YUM_OUTATR YumVector* YumVector_new(void);

/**
 * @brief Frees a YumVector and its contained elements.
 *
 * @param vec Pointer to a YumVector created by YumVector_new().
 */

YUM_OUTATR void YumVector_delete(YumVector* vec);

/**
 * @brief Appends a value to the vector.
 *
 * @param vec   Target vector.
 * @param value Value to append (copied from the given YumVariant).
 */

YUM_OUTATR void YumVector_append(YumVector* vec, const YumVariant* value);

/**
 * @brief Clears all elements from the vector.
 *
 * @param vec Target vector.
 */

YUM_OUTATR void YumVector_clear(YumVector* vec);

/**
 * @brief Returns the number of elements in the vector.
 *
 * @param vec Source vector.
 * @return The current size of the vector.
 */

YUM_OUTATR int64_t YumVector_size(const YumVector* vec);

/**
 * @brief Retrieves a modifiable element at the specified index.
 *
 * @param vec   Target vector.
 * @param index Zero-based index.
 *
 * @return Pointer to the YumVariant at that position (borrowed reference — do not free).
 */

YUM_OUTATR YumVariant* YumVector_at(YumVector* vec, int64_t index);

/**
 * @brief Retrieves a const element at the specified index.
 *
 * @param vec   Source vector.
 * @param index Zero-based index.
 *
 * @return Const pointer to the YumVariant at that position (borrowed reference — do not free).
 */

YUM_OUTATR const YumVariant* YumVector_at_const(const YumVector* vec, int64_t index);

#ifdef __cplusplus
}
#endif
#endif // YUM_CXX_API_NO_C_EXPORT
#endif // ___YUM_VECTOR_H___