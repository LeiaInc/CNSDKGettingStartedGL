#pragma once

#include "leia/common/defines.h"
#include "leia/common/config.h"

#ifdef LEIA_COMMON_EXPORTS
#    define LEIA_COMMON_API LEIA_EXPORT
#    define LEIA_COMMON_CLASS_API LEIA_CLASS_EXPORT
#else
#    ifdef LEIA_COMMON_STATIC
#        define LEIA_COMMON_API
#        define LEIA_COMMON_CLASS_API
#    else
#        define LEIA_COMMON_API LEIA_IMPORT
#        define LEIA_COMMON_CLASS_API LEIA_CLASS_IMPORT
#    endif
#endif
