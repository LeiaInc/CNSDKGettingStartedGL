#pragma once

#include "leia/common/types.hpp"

#include <functional>

namespace leia {

struct SharedCameraSink {
    std::function<void(ImageDesc const&, Timestamp timestamp)> imageCallback;

    /// Should be called whenever the camera's intrinsics change, on the same thread as imageCallback.
    std::function<void(CameraIntrinsics const&)> intrinsicsCallback;
};

} // namespace leia
