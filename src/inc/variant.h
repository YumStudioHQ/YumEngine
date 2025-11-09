/* variant.h — Yum C API wrapper
 ** Exposes Yumcxx::Variant for C and dynamic loading
 */
#pragma once
#include <stdint.h>

/* export macro */
#include "yumdec.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct YumVariant YumVariant; /* Opaque struct */

/* lifecycle */
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    YumVariant*    YumVariant_new(void);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    void           YumVariant_delete(YumVariant* var);

/* setters */
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    void           YumVariant_setInt(YumVariant* var, int64_t v);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    void           YumVariant_setFloat(YumVariant* var, double v);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    void           YumVariant_setBool(YumVariant* var, int32_t v); /* use 0/1 */
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    void           YumVariant_setString(YumVariant* var, const char* str);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    void           YumVariant_setRawBytes(YumVariant* var, const uint8_t* str, uint64_t len);

/* getters */
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    int64_t        YumVariant_asInt(const YumVariant* var);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    double         YumVariant_asFloat(const YumVariant* var);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    int            YumVariant_asBool(const YumVariant* var);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    const char*    YumVariant_asString(const YumVariant* var);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    YumBinaryBlob  Yumvariant_asBinary(const YumVariant* var);

/* type checks */
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    int32_t        YumVariant_isInt(const YumVariant* var);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    int32_t        YumVariant_isFloat(const YumVariant* var);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    int32_t        YumVariant_isBool(const YumVariant* var);
YUM_METADATA(YUM_VERSION(1.9)) YUM_OUTATR    int32_t        YumVariant_isString(const YumVariant* var);

#ifdef __cplusplus
}
#endif
