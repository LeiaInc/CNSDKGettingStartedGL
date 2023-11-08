#pragma once

#include "leia/common/api.h"

#include <stdint.h>

#define CNSDK_VERSION "0.7.99"
#define CNSDK_MAJOR_VERSION 0
#define CNSDK_MINOR_VERSION 7
#define CNSDK_PATCH_VERSION 99

typedef uint64_t leia_version;

#define LEIA_MAKE_VERSION(major, minor, patch) \
    ((((major) & 0xffffULL) << 48) | (((minor) & 0xffffULL) << 32) | ((patch) & 0xffffffffULL))

#define LEIA_VERSION_MAJOR(version) (uint16_t)(((uint64_t)(version) >> 48)& 0xffffULL)
#define LEIA_VERSION_MINOR(version) (uint16_t)(((uint64_t)(version) >> 32) & 0xffffULL)
#define LEIA_VERSION_PATCH(version) (uint32_t)((uint64_t)(version) & 0xffffffffULL)

#define CNSDK_VERSION_U64 LEIA_MAKE_VERSION(CNSDK_MAJOR_VERSION, CNSDK_MINOR_VERSION, CNSDK_PATCH_VERSION)

BEGIN_CAPI_DECL

LEIA_COMMON_API
const char* leia_get_git_refspec();

LEIA_COMMON_API
const char* leia_get_git_sha1();

LEIA_COMMON_API
const char* leia_get_version();

END_CAPI_DECL
