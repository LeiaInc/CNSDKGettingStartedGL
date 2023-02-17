#pragma once

#include "leia/device/api.h"

#define LEIADEVICE_VERSION "0.1.1"
#define LEIADEVICE_MAJOR_VERSION 0
#define LEIADEVICE_MINOR_VERSION 1
#define LEIADEVICE_PATCH_VERSION 1

BEGIN_CAPI_DECL

LEIADEVICE_API
const char* leiaDeviceGetVersion();

END_CAPI_DECL
