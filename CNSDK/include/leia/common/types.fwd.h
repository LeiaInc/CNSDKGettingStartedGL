#ifndef CNSDK_LEIA_COMMON_TYPES_FWD_H
#define CNSDK_LEIA_COMMON_TYPES_FWD_H

#include "leia/common/defines.h"

BEGIN_CAPI_DECL

/// Defines an interface for an external user to notify CNSDK of camera events.
struct leia_shared_camera_sink;
struct leia_core_loader;
struct leia_core;
struct leia_event_listener;
struct leia_event_center;
struct leia_platform_configuration;
struct leia_const_float_slice;
struct leia_float_slice;
struct leia_slice;
struct leia_asset_manager;

END_CAPI_DECL

#endif // CNSDK_LEIA_COMMON_TYPES_FWD_H
