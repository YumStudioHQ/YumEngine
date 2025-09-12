
#include <stdint.h>

#include "yumdec.h"
#include "variant.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward declare opaque types
typedef struct YumVector YumVector;

// Creation & destruction
YUM_OUTATR YumVector* YumVector_new(void);
YUM_OUTATR void YumVector_delete(YumVector* vec);

// Operations
YUM_OUTATR void YumVector_append(YumVector* vec, const YumVariant* value);
YUM_OUTATR void YumVector_clear(YumVector* vec);
YUM_OUTATR int64_t YumVector_size(const YumVector* vec);

// Access (borrowed reference — don't free!)
YUM_OUTATR YumVariant* YumVector_at(YumVector* vec, int64_t index);
YUM_OUTATR const YumVariant* YumVector_at_const(const YumVector* vec, int64_t index);

#ifdef __cplusplus
}
#endif
