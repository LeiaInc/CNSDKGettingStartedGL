#ifndef CNSDK_LEIA_CORE_EVENT_H
#define CNSDK_LEIA_CORE_EVENT_H

#include "leia/common/defines.h"

BEGIN_CAPI_DECL

enum leia_core_event_code {
    LEIA_CORE_EVENT_DID_INITIALIZE  = 0, ///< leia_core is fully initialized, payload: NONE
    LEIA_CORE_EVENT_INVALID_LICENSE = 1, ///< leia_core detected invalid license, payload: [optional] const char* reason - describes what exactly went wrong
    _LEIA_CORE_EVENT_CODE_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

enum leia_interlacer_event_code {
    LEIA_INTERLACER_EVENT_DEBUG_MENU_CLOSED     = 0, ///< leia_interlacer's debug menu has been closed, payload: NONE
    LEIA_INTERLACER_EVENT_DEBUG_MENU_UPDATE     = 1, ///< leia_interlacer's debug menu has changed a value, payload: [optional] const char* reason - value's id
    _LEIA_INTERLACER_EVENT_CODE_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

END_CAPI_DECL

#endif // CNSDK_LEIA_CORE_EVENT_H
