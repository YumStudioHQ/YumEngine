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
YUM_OUTATR YumVariant* YumVariant_new(void);
YUM_OUTATR void        YumVariant_delete(YumVariant* var);

/* setters */
YUM_OUTATR void YumVariant_setInt(YumVariant* var, int64_t v);
YUM_OUTATR void YumVariant_setFloat(YumVariant* var, double v);
YUM_OUTATR void YumVariant_setBool(YumVariant* var, int v); /* use 0/1 */
YUM_OUTATR void YumVariant_setString(YumVariant* var, const char* str);

/* getters */
YUM_OUTATR int64_t     YumVariant_asInt(const YumVariant* var);
YUM_OUTATR double      YumVariant_asFloat(const YumVariant* var);
YUM_OUTATR int         YumVariant_asBool(const YumVariant* var);
YUM_OUTATR const char* YumVariant_asString(const YumVariant* var); 

/* type checks */
YUM_OUTATR int32_t YumVariant_isInt(const YumVariant* var);
YUM_OUTATR int32_t YumVariant_isFloat(const YumVariant* var);
YUM_OUTATR int32_t YumVariant_isBool(const YumVariant* var);
YUM_OUTATR int32_t YumVariant_isString(const YumVariant* var);

#ifdef __cplusplus
}
#endif
