#pragma once

#include "leia/headTracking/engine/api.h"
#include "leia/headTracking/common/types.hpp"

#include <string>
#include <memory>

namespace leia { class AssetManager; }

namespace leia::head {

class FaceDetector;
struct FaceDetectorConfiguration;

static constexpr int kBlinkDetectorMaxNumOfFacesLimit = 3;

struct BlinkDetectorConfiguration {
    FaceDetectorConfiguration const* common;
    /// A path pointing to a log file of Blink library. The default path is used if empty.
    std::string logFilePath;
};

LHT_ENGINE_API
std::shared_ptr<FaceDetector> CreateBlinkDetector(BlinkDetectorConfiguration const&, AssetManager*);

LHT_ENGINE_API
bool IsBlinkDetectorBackendSupported(FaceDetectorBackend);

} // namespace leia::head
