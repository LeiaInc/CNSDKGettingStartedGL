#ifndef CNSDK_LEIA_HEADTRACKING_COMMON_EVENT_H
#define CNSDK_LEIA_HEADTRACKING_COMMON_EVENT_H

#include "leia/common/defines.h"

BEGIN_CAPI_DECL

enum leia_face_tracking_event_code_e {
    /// Face tracking operation has failed and the face tracking backend enountered a fatal error.
    /// An attempt at recovery may be possible by shutting down the face tracking and trying to enable it again.
    /// Payload - const char* errorMessage.
    LEIA_FACE_TRACKING_FATAL_ERROR                 = 0,
    LEIA_FACE_TRACKING_ERROR                       = 1,
    LEIA_FACE_TRACKING_STARTED                     = 2,
    LEIA_FACE_TRACKING_STOPPED                     = 3,
    _LEIA_FACE_TRACKING_EVENT_CODE_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

END_CAPI_DECL

#endif // CNSDK_LEIA_HEADTRACKING_COMMON_EVENT_H
