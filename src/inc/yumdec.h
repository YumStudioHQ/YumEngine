#ifndef ___YUM_DEC_H___
#define ___YUM_DEC_H___

#ifdef _WIN32
#  define YUM_OUTATR __declspec(dllexport)
#else
#  define YUM_OUTATR __attribute__((visibility("default")))
#endif

enum YumCode {
  YUM_OK,
  YUM_ERROR,
  YUM_NOT_INIT,
};

#endif // ___YUM_DEC_H___