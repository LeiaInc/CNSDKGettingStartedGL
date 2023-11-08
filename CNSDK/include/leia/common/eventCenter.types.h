#ifndef CNSDK_LEIA_COMMON_EVENT_CENTER_TYPES_H
#define CNSDK_LEIA_COMMON_EVENT_CENTER_TYPES_H

#include "leia/common/log.h"
#include "leia/common/types.fwd.h"

#include <stdint.h>

BEGIN_CAPI_DECL

#pragma pack(push, 8)

enum leia_event_type {
    LEIA_EVENT_TYPE_LOG              = 0,
    LEIA_EVENT_TYPE_ERROR            = 1,
    LEIA_EVENT_TYPE_COMPONENT        = 2,
    _LEIA_EVENT_TYPE_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

struct leia_event_log {
    const char*    message;
    leia_log_level level;
};

enum leia_component_id {
    LEIA_COMPONENT_ID_UNDETERMINED     = 0,
    LEIA_COMPONENT_ID_CORE             = 1,
    LEIA_COMPONENT_ID_FACE_TRACKING    = 2,
    LEIA_COMPONENT_ID_INTERLACER       = 3,
    _LEIA_COMPONENT_ID_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

struct leia_event_error {
    const char*            message;
    int64_t                code;
    enum leia_component_id component;
};

struct leia_event_component {
    enum leia_component_id id; ///< The component id
    int32_t                code; ///< The component-specific code
    void const*            payload; ///< The component-specific payload, if any
};

enum leia_error_type {
    LEIA_ERROR_INVALID_USE           = -1,
    LEIA_ERROR_INTERNAL_ERROR        = -2,
    LEIA_ERROR_OUT_OF_SYSTEM_MEMORY  = -3,
    LEIA_ERROR_ASSERTION             = -4,
    _LEIA_ERROR_TYPE_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

struct leia_event {
    enum leia_event_type type;
    union {
        struct leia_event_log       log;
        struct leia_event_error     error;
        struct leia_event_component component;
    } v;
};

typedef void (*leia_event_listener_callback)(void* userData, struct leia_event const*);

#pragma pack(pop)

END_CAPI_DECL

#endif // CNSDK_LEIA_COMMON_EVENT_CENTER_TYPES_H
