#ifndef CNSDK_LEIA_CORE_ASSET_MANAGER_H
#define CNSDK_LEIA_CORE_ASSET_MANAGER_H

#include "leia/core/api.h"

BEGIN_CAPI_DECL

/**
 * @brief Get experimental API entry point.
 *
 * CNSDK is built with ABI-stability in mind.
 * This limits the API's evolution.
 * Some part of the API is not set in stone (in ABI),
 * and can be removed/changed in future releases.
 * This function allows accessing such API entry points.
 *
 */
LEIA_NODISCARD
LEIA_COMMON_API
void* leia_get_experimental_api(struct leia_core_library*, const char* name);

#define LEIA_GET_EXPERIMENTAL_API(coreLib, T) (T) leia_get_experimental_api(coreLib, #T);

END_CAPI_DECL

#endif // CNSDK_LEIA_CORE_ASSET_MANAGER_H
