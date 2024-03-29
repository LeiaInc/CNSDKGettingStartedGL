#pragma once

#include "leia/common/api.h"

#include <stdint.h>

BEGIN_CAPI_DECL

enum // leia_log_level
{
    kLeiaLogLevelDefault = 0,
    kLeiaLogLevelTrace,
    kLeiaLogLevelDebug,
    kLeiaLogLevelInfo,
    kLeiaLogLevelWarn,
    kLeiaLogLevelError,
    kLeiaLogLevelCritical,
    kLeiaLogLevelOff,
    kNumLeiaLogLevels
};

typedef int32_t leia_log_level;

LEIA_NODISCARD
inline const char* leia_log_level_to_string(leia_log_level logLevel)
{
    switch (logLevel)
    {
        case kLeiaLogLevelDefault:
            return "Default";
        case kLeiaLogLevelTrace:
            return "Trace";
        case kLeiaLogLevelDebug:
            return "Debug";
        case kLeiaLogLevelInfo:
            return "Info";
        case kLeiaLogLevelWarn:
            return "Warn";
        case kLeiaLogLevelError:
            return "Error";
        case kLeiaLogLevelCritical:
            return "Critical";
        case kLeiaLogLevelOff:
            return "Off";
        default:
            return "Unknown";
    }
}

END_CAPI_DECL
