#pragma once

#include "leia/headTracking/engine/core/faceDetector.hpp"

#include <string>
#include <memory>

namespace leia { class AssetManager; }

namespace leia::head {

static constexpr int kBlinkDetectorMaxNumOfFacesLimit = 3;

struct BlinkDetectorConfiguration {
    FaceDetectorConfiguration common;
    /// A path pointing to a log file of Blink library. The default path is used if empty.
    std::string logFilePath;

    void* eglDisplay = nullptr;
    void* eglContext = nullptr;
};

LHT_ENGINE_API
std::shared_ptr<FaceDetector> CreateBlinkDetector(BlinkDetectorConfiguration const&, AssetManager*);

LHT_ENGINE_API
void UpdateBlinkDetectorConfig(FaceDetector*, BlinkDetectorConfiguration const&);

LHT_ENGINE_API
bool IsBlinkDetectorBackendSupported(FaceDetectorBackend);

} // namespace leia::head
