/*
 * Copyright 2023 (c) Leia Inc.  All rights reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Leia Inc. and its suppliers, if any.  The
 * intellectual and technical concepts contained herein are
 * proprietary to Leia Inc. and its suppliers and may be covered
 * by U.S. and Foreign Patents, patents in process, and are
 * protected by trade secret or copyright law.  Dissemination of
 * this information or reproduction of this materials strictly
 * forbidden unless prior written permission is obtained from
 * Leia Inc.
 */
#pragma once

#include "leia/common/api.h"

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
void* leia_get_experimental_api(const char* name);

#define LEIA_GET_EXPERIMENTAL_API(T) (T) leia_get_experimental_api(#T);

END_CAPI_DECL
