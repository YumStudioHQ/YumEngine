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

typedef struct {
  const uint8_t *start;
  uint64_t       size;
} YumBinaryBlob;

#endif // ___YUM_DEC_H___