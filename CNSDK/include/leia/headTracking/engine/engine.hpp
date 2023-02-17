#pragma once

#include "leia/headTracking/engine/core/camera.hpp"
#include "leia/headTracking/engine/core/faceTracker.hpp"
#include "leia/headTracking/engine/core/faceDetector.hpp"

#include "leia/common/jniTypes.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <functional>

namespace leia {

class Platform;
class AssetManager;
struct SharedCameraSink;

#if defined(LEIA_OS_ANDROID)
namespace device { class SensorManager; }
#endif

} // namespace leia

namespace leia::head {

struct BlinkDetectorConfiguration;
class AndroidCameraSinkGPU;
class AndroidCameraSinkCPU;
class FrameAdapter;

struct EngineConfiguration {
    FaceTracker::Configuration faceTracker;
    FaceDetectorConfiguration faceDetector;
    CameraConfiguration camera;

    bool initializeFaceDetectorLazily;
    bool enableProfiling;

    LHT_ENGINE_API
    EngineConfiguration();
};

using EngineFrameCallback = std::function<void(FaceTracker const&, FaceDetector::Output const&, CameraFrame const&)>;

struct VirtualFace {
    glm::vec3 trackingPoint;
    float interocularDistance;
};
struct VirtualFaceHookContext {
    // Current timestamp.
    const Timestamp timestamp;
    // Current number of detected faces by the face detector.
    const int numDetectedFaces;
    // The maximum number of virtual faces that can be filled by the hook.
    const int maxNumVirtualFaces;
    // Buffer to be filled by virtual face hook.
    VirtualFace* const virtualFaces;
    // Actual number of virtual faces filled by the hook.
    int numVirtualFaces;
    // Whether to drop real detected faces or combine them with virtual faces.
    bool dropDetectedFaces;
};
// Returns true if the hook is active.
using VirtualFaceHook = std::function<bool(VirtualFaceHookContext* ctx)>;

struct EngineInitArgs {
    EngineConfiguration configuration;

    // Must be set
    Platform* platform = nullptr;

    // Optional. Engine will create its own asset manager if not provided.
    AssetManager* assetManager = nullptr;

    /// FrameCallback invoked after each processed CameraFrame.
    /// It's always called from the background thread where face detection happens.
    /// (Except the case when sharedCameraSink != nullptr. See SharedCameraSink for more info.)
    /// Therefore it's advised to keep FrameCallback as fast as possible.
    EngineFrameCallback frameCallback = nullptr;

    /// See FrameAdapter declaration for info.
    FrameAdapter* frameAdapter = nullptr;

    /// Allows managing the camera externally. See SharedCameraSink for more info.
    /// The user guarantees the validity of the pointer through the Engine's lifetime.
    SharedCameraSink* sharedCameraSink = nullptr;

    /// Use this to inject virtual detected face into the face tracking algorithm. Useful for debugging the multi-face case.
    VirtualFaceHook virtualFaceHook = nullptr;

    TrackingStateListener* trackingStateListener = nullptr;
};

struct TrackingSessionInitArgs {
    /// Android only.
    /// If set to a valid android.view.Surface object, android camera will use this as an additional output.
    /// Thus this surface should meet compatibility requirements:
    ///   * YUV or private format
    ///   * Size should be one of ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS
    /// Engine acquires strong reference to the object while tracking is active.
    /// Unused if the shared camera is in use.
    jobject androidSurface = nullptr;
};

/// Manages all head tracking components.
/// On Android, it's expected for the host application to request and handle camera permission.
/// Engine's API is not thread-safe.
class Engine final : public Camera::Listener {
public:
    LHT_ENGINE_API
    Engine(EngineInitArgs const&);
    LHT_ENGINE_API
    ~Engine();

    LHT_ENGINE_API
    bool StartTracking();
    LHT_ENGINE_API
    bool StartTracking(TrackingSessionInitArgs const&);
    LHT_ENGINE_API
    bool StopTracking() noexcept;

    LHT_ENGINE_API
    void SetTrackedEyes(bool left, bool right);

    /// Configures camera transform using Leia Display Config values.
    /// Can be overriden using SetCameraPosition, SetCameraRotation.
    LHT_ENGINE_API
    void SetCameraTransform(glm::vec3 const& cameraPosition, glm::vec3 const& cameraRotation);
    LHT_ENGINE_API
    void SetCameraPosition(glm::vec3 const& position);
    LHT_ENGINE_API
    void SetCameraRotation(
        float r00, float r01, float r02,
        float r10, float r11, float r12,
        float r20, float r21, float r22);
    LHT_ENGINE_API
    void SetFilterProperties(FilterProperties const&);

    LHT_ENGINE_API
    int GetMaxNumOfDetectedFacesLimit() const;
    LHT_ENGINE_API
    void SetMaxNumOfDetectedFaces(int);

    LHT_ENGINE_API
    bool SetFaceDetectorBackend(FaceDetectorBackend);
    LHT_ENGINE_API
    FaceDetectorBackend GetFaceDetectorBackend() const;
    LHT_ENGINE_API
    bool SetFaceDetectorInputType(FaceDetectorInputType);
    LHT_ENGINE_API
    FaceDetectorInputType GetFaceDetectorInputType() const;

    LHT_ENGINE_API
    Slice<const int> GetSupportedCameraFps() const;
    LHT_ENGINE_API
    bool SetCameraFps(int fps);
    LHT_ENGINE_API
    int GetCameraFps() const;

    LHT_ENGINE_API
    void SetProfiling(bool enable);

private:
    enum ChangeTracker {
        Clean = 0,
        AllDirty = ~Clean,
        DirtyFaceTracker = 1 << 0,
        DirtyMaxNumOfFaces = 1 << 1,
    };

    void OnCameraFrameAvailable(Camera* camera, CameraFrame const& cameraFrame);
    void OnCameraIntrinsicsChange(Camera* camera, CameraIntrinsics const& intrinsics);

    bool OnFaceDetectorConfigChange();
    void UpdateFaceDetectorConfig();
    void UpdateCameraSink();

    void CreateCamera();

    void ProcessFrame(CameraFrame const& cameraFrame);

    void CreateFaceDetector();

    float GetDeviceAcceleration();
    void PushDeviceAcceleration(glm::vec3 const&);

private:
    Platform* _platform;
    AssetManager* _assetManager;
    TrackingStateListener* _trackingStateListener;
    EngineConfiguration _config;
    EngineFrameCallback _frameCallback;
    FrameAdapter* _frameAdapter;

    VirtualFaceHook _virtualFaceHook;
    std::vector<VirtualFace> _virtualFacesBuffer;

    CameraIntrinsics _cameraIntrinsics;

#if defined(LEIA_OS_ANDROID)
    std::mutex _rollingMaxAccelerationMutex;
    float _lastAcceleration = 0.0f;
    float _rollingMaxAcceleration = 0.0f;
    int _numRollingMaxAccelerationSamples = 0;

    std::unique_ptr<device::SensorManager> _sensorManager;
#endif

    bool _isProfilingEnabled = false;
    std::atomic<bool> _trackingStarted;
    bool _notifyTrackingStart = false;

    std::mutex _configMutex;
    std::atomic<uint32_t> _dirtyBits = AllDirty;

    std::shared_ptr<FaceDetector> _faceDetector;
    std::unique_ptr<FaceTracker> _faceTracker;

#if defined(LEIA_OS_ANDROID)
    std::unique_ptr<AndroidCameraSinkCPU> _cpuSink;
    std::unique_ptr<AndroidCameraSinkGPU> _gpuSink;
#endif

    SharedCameraSink* _sharedCameraSink;
    std::atomic<bool> _isCameraValid;
    std::unique_ptr<Camera> _camera; // Camera should be the first to release, keep it at the bottom

    friend BlinkDetectorConfiguration GetBlinkDetectorConfiguration(Engine*);
};

LHT_ENGINE_API
bool IsCameraConnected();

LHT_ENGINE_API
uint32_t GetSupportedFaceDetectorBackends();

} // namespace leia::head
