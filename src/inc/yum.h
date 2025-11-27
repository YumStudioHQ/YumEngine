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

/**
 * @file yum.hpp
 * @brief Includes all headers for the YumEngine C API.
 */

#ifndef ___YUM_H___
#define ___YUM_H___

#ifndef YUM_CXX_API_NO_C_EXPORT

#include "variant.h"
#include "vector.h"
#include "yumtable.h"
#include "subsystem.h"

void YumEngine_init(void);

#endif // YUM_CXX_API_NO_C_EXPORT
#endif // ! ___YUM_H___