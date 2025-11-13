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
#ifndef ___YUM_DEC_H___
#define ___YUM_DEC_H___

#include <stdint.h>

#ifdef _WIN32
#  define YUM_OUTATR __declspec(dllexport)
#else
#  define YUM_OUTATR __attribute__((visibility("default")))
#endif

#define YUM_METADATA(...)
#define YUM_VERSION(x)

enum YumCode {
  YUM_OK,
  YUM_ERROR,
  YUM_NOT_INIT,
};

/**
 * @brief Safe binary-blob type.
 */
typedef struct {
  const uint8_t *start;
  uint64_t       size;
} YumBinaryBlob;

/**
 * @brief data representing an UID. It promises you 8 bytes, managed as an integer in Lua.
 */
typedef struct {
  int64_t bytes;
} YumUID;

#endif // ___YUM_DEC_H___