#pragma once

#include "leia/core/api.h"
#include "leia/common/types.h"
#include "leia/common/version.h"
#include "leia/common/jniTypes.h"

BEGIN_CAPI_DECL

#if defined(LEIA_DOXYGEN)
/**
 * @brief Core library.
 *
 * Represents a loaded core library.
 * One must be always obtained before using any of the core functionality.
 */
struct leia_core_library {};
#endif

struct leia_core_library_load_android {
    JavaVM* vm;
    jobject context; // android/content/Context
};

#define LEIA_CORE_LOADER_API_VERSION 1
struct leia_core_library_load_request {
    leia_version apiVersion; // CNSDK_VERSION_U64
    uint32_t     loaderVersion; // LEIA_CORE_LOADER_API_VERSION

    leia_core_library_load_android const* android;
};

LEIASDK_API
struct leia_core_library* leia_core_library_load(leia_core_library_load_request* request);

LEIASDK_API
void leia_core_library_release(struct leia_core_library*);

END_CAPI_DECL
