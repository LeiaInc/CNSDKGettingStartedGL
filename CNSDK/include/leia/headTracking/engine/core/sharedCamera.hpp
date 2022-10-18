#pragma once

#include "leia/headTracking/engine/core/camera.hpp"

#include <functional>

namespace leia { struct SharedCameraSink; }

namespace leia::head {

/// SharedCamera allows a user to manage the camera.
///
/// All of the face tracking is driven by SharedCameraSink::imageCallback.
/// Meaning face detection and face tracking happens in SharedCameraSink::imageCallback.
/// EngineFrameCallback/FrameAdapter::Callback is invoked right at the end of SharedCameraSink::imageCallback.
class SharedCamera : public Camera {
public:
    LHT_ENGINE_API
    SharedCamera(Listener* listener, SharedCameraSink* sharedCameraSink);
    LHT_ENGINE_API
    ~SharedCamera() override;

    void StartCapture() override {};
    void StopCapture() override {};

    int GetFps() const override { return 0; }
    bool SetFps(int) override { return false; }
    Slice<const int> GetSupportedFps() const override { return {}; }

private:
    Listener* _listener;
    SharedCameraSink* _sharedCameraSink;
};

} // namespace leia::head
