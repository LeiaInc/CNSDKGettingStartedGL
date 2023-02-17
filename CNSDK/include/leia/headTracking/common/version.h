#pragma once

#include "leia/headTracking/common/api.h"

#define LEIAHEADTRACKING_VERSION "0.6.84"
#define LEIAHEADTRACKING_MAJOR_VERSION 0
#define LEIAHEADTRACKING_MINOR_VERSION 6
#define LEIAHEADTRACKING_PATCH_VERSION 84

BEGIN_CAPI_DECL

LHT_COMMON_API
const char* leiaHeadTrackingGetVersion();

END_CAPI_DECL
