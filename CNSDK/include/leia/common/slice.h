#ifndef CNSDK_LEIA_COMMON_SLICE_H
#define CNSDK_LEIA_COMMON_SLICE_H

#include "leia/common/types.fwd.h"

#include <stdint.h>

BEGIN_CAPI_DECL

#pragma pack(push, 8)

struct leia_float_slice {
    float*  data;
    int32_t length;
};

struct leia_const_float_slice {
    float const* data;
    int32_t      length;
};

struct leia_slice {
    void*   data;
    int32_t length;
};

#define LEIA_FLOAT_SLICE(size) struct leia_float_slice
#define LEIA_CONST_FLOAT_SLICE(size) struct leia_const_float_slice

#pragma pack(pop)

END_CAPI_DECL

#endif // CNSDK_LEIA_COMMON_SLICE_H
