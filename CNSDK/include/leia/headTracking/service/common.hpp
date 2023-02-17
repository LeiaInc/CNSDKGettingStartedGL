#pragma once

#include "leia/headTracking/common/types.hpp"
#include "leia/common/log.h"

#include <string>

namespace leia::head {

enum ServiceSpecificError {
    kServiceSuccess,
    kInvalidFrame,
    kFailedToRegisterListener,
    kInvalidListenerId,
    kFailedToInitializeEngine,
    kFailedToStartTracking,
    kFailedToStopTracking,
    kInvalidValue,
    kInvalidProtocol,
    kNotSupported,
    kGenericError,
    kCameraPermissionDenied,
};

struct ServerInfo {
    // Added in version 0.4.1
    std::string version;
    uint32_t supportedFaceDetectorBackends;
};

const char* ToStr(ServiceSpecificError error);

struct ServerInitArgs {
    /// if set to a non-default level, we request the service to change its log level to the specified while the current session persists
    leia_log_level logLevel = kLeiaLogLevelDefault;

    /// if set to a non-unknown backend, we request the service to change its backend to the specified
    FaceDetectorBackend faceDetectorBackend = kLeiaFaceDetectorBackendUnknown;

    /// if set to a non-unknown input type, we request the service to change its input type to the specified
    FaceDetectorInputType faceDetectorInputType = kLeiaFaceDetectorInputTypeUnknown;

    bool enableProfiling = false;
};

} // namespace leia::head
