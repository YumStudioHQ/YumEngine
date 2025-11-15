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

#ifndef ___YUMGUARD_VARIANT_H___
#define ___YUMGUARD_VARIANT_H___
#include <stdint.h>

/* export macro */
#include "yumdec.h"

/**
 * @file variant.h
 * @brief C API for Yum Variant — dynamic typed value container.
 *
 * This header provides a C wrapper around `Yumcxx::Variant`, which is a 
 * dynamically typed container used to store integers, floating-point values, 
 * booleans, strings, or raw binary data.
 *
 * Variants are used extensively throughout the Yum API to pass values between
 * Lua, native code, and user callbacks.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque handle to a Yum Variant.
 */
typedef struct YumVariant YumVariant;

/**
 * @brief Allocates a new empty variant (nil value).
 *
 * @return Pointer to a new YumVariant instance.
 *         Must be freed with YumVariant_delete().
 */

YUM_OUTATR YumVariant* YumVariant_new(void);

/**
 * @brief Frees a YumVariant and its resources.
 *
 * @param var Pointer to a YumVariant instance created with YumVariant_new().
 */

YUM_OUTATR void YumVariant_delete(YumVariant* var);

/** @name Setters
 *  Functions for assigning values to a YumVariant.
 *  @{
 */

/**
 * @brief Sets the variant to an integer value.
 * @param var Target variant.
 * @param v   Integer value to assign.
 */

YUM_OUTATR void YumVariant_setInt(YumVariant* var, int64_t v);

/**
 * @brief Sets the variant to a floating-point value.
 * @param var Target variant.
 * @param v   Floating-point value to assign.
 */

YUM_OUTATR void YumVariant_setFloat(YumVariant* var, double v);

/**
 * @brief Sets the variant to a boolean value.
 * @param var Target variant.
 * @param v   Integer value (0 = false, nonzero = true).
 */

YUM_OUTATR void YumVariant_setBool(YumVariant* var, int32_t v);

/**
 * @brief Sets the variant to a UTF-8 string value.
 * @param var Target variant.
 * @param str Null-terminated C string.
 */

YUM_OUTATR void YumVariant_setString(YumVariant* var, const char* str);

/**
 * @brief Sets the variant to raw binary data.
 * @param var Target variant.
 * @param data Pointer to bytes.
 * @param len  Length of the binary blob in bytes.
 */

YUM_OUTATR void YumVariant_setRawBytes(YumVariant* var, const uint8_t* data, uint64_t len);

/** @} */ // end setters

/** @name Getters
 *  Functions for retrieving values from a YumVariant.
 *  @{
 */

/**
 * @brief Retrieves the variant as an integer.
 * @param var Source variant.
 * @return Integer value (undefined if type is not integer).
 */

YUM_OUTATR int64_t YumVariant_asInt(const YumVariant* var);

/**
 * @brief Retrieves the variant as a floating-point number.
 * @param var Source variant.
 * @return Floating-point value.
 */

YUM_OUTATR double YumVariant_asFloat(const YumVariant* var);

/**
 * @brief Retrieves the variant as a boolean.
 * @param var Source variant.
 * @return 0 (false) or 1 (true).
 */

YUM_OUTATR int YumVariant_asBool(const YumVariant* var);

/**
 * @brief Retrieves the variant as a string.
 * @param var Source variant.
 * @return Pointer to a UTF-8 string owned by the variant (do not free).
 */

YUM_OUTATR const char* YumVariant_asString(const YumVariant* var);

/**
 * @brief Retrieves the variant as a binary blob.
 * @param var Source variant.
 * @return A YumBinaryBlob structure describing the binary data.
 */

YUM_OUTATR YumBinaryBlob YumVariant_asBinary(const YumVariant* var);

/** @} */ // end getters

/** @name Type checks
 *  Functions to determine the stored type of a YumVariant.
 *  @{
 */

/**
 * @brief Checks if the variant holds an integer.
 */

YUM_OUTATR int32_t YumVariant_isInt(const YumVariant* var);

/**
 * @brief Checks if the variant holds a float.
 */

YUM_OUTATR int32_t YumVariant_isFloat(const YumVariant* var);

/**
 * @brief Checks if the variant holds a boolean.
 */

YUM_OUTATR int32_t YumVariant_isBool(const YumVariant* var);

/**
 * @brief Checks if the variant holds a string.
 */

YUM_OUTATR int32_t YumVariant_isString(const YumVariant* var);

/** @} */ // end type checks

#ifdef __cplusplus
}
#endif
#endif // ___YUMGUARD_VARIANT_H___