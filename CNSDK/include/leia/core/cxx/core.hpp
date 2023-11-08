#ifndef CNSDK_LEIA_CORE_CXX_CORE_HPP
#define CNSDK_LEIA_CORE_CXX_CORE_HPP

#include "leia/core/core.h"
#include "leia/core/cxx/library.hpp"
#include "leia/core/cxx/types.hpp"
#include "leia/core/cxx/exception.hpp"
#include "leia/core/cxx/assetManager.hpp"
#include "leia/common/version.h"

/*!
 * @addtogroup core
 * @{
 */

namespace leia {

using Vector3 = leia_vector3;
using Vector4 = leia_vector4;
using Mat4    = leia_mat4;

using FloatSlice = leia_float_slice;
inline FloatSlice ToSlice(Vector3* v)
{
    return {v->v, 3};
}
inline FloatSlice ToSlice(Mat4* v)
{
    return {v->m, 16};
}
using ConstFloatSlice = leia_const_float_slice;
inline ConstFloatSlice ToConstSlice(leia_vector3 const* v)
{
    return {v->v, 3};
}
inline ConstFloatSlice ToConstSlice(Mat4* v)
{
    return {v->m, 16};
}

using SharedCameraSink = leia_shared_camera_sink;

namespace device {
using Config = leia_device_config;
} // namespace device

class Core;

using CoreAndroidHandleType = leia_core_android_handle_type;

using FaceTrackingFrameListener = leia_headtracking_frame_listener;
using FaceTrackingFrame         = leia_headtracking_frame;

/**
 * @brief C++ wrapper for leia_core_init_configuration
 */
class CoreInitConfiguration {
public:
    CoreInitConfiguration(CoreLibrary const& coreLib) : _ptr(leia_core_init_configuration_alloc(coreLib.GetHandle(), CNSDK_VERSION))
    {
    }
    ~CoreInitConfiguration()
    {
        leia_core_init_configuration_free(_ptr);
    }
    CoreInitConfiguration(CoreInitConfiguration const&) = delete;
    CoreInitConfiguration& operator=(CoreInitConfiguration const&) = delete;
    CoreInitConfiguration(CoreInitConfiguration&&) = delete;
    CoreInitConfiguration& operator=(CoreInitConfiguration&&) = delete;

    void SetLicenseKey(const char* licenseKey)
    {
        leia_core_init_configuration_set_license_key(_ptr, licenseKey);
    }
    void SetLicenseNetworkProxy(const char* networkProxy)
    {
        leia_core_init_configuration_set_license_network_proxy(_ptr, networkProxy);
    }
    void SetEnableValidation(bool enable)
    {
        leia_core_init_configuration_set_enable_validation(_ptr, enable);
    }
    void SetPlatformAndroidHandle(CoreAndroidHandleType handleType, jobject handle)
    {
        leia_core_init_configuration_set_platform_android_handle(_ptr, handleType, handle);
    }
    void SetPlatformLogLevel(leia_log_level logLevel)
    {
        leia_core_init_configuration_set_platform_log_level(_ptr, logLevel);
    }
    void SetFaceTrackingSharedCameraSink(leia_shared_camera_sink* sharedCameraSink)
    {
        leia_core_init_configuration_set_face_tracking_shared_camera_sink(_ptr, sharedCameraSink);
    }
    void SetFaceTrackingRuntime(leia_face_tracking_runtime_type runtime)
    {
        leia_core_init_configuration_set_face_tracking_runtime(_ptr, runtime);
    }
    void SetFaceTrackingEnable(bool enable)
    {
        leia_core_init_configuration_set_face_tracking_enable(_ptr, enable);
    }
    void SetFaceTrackingStart(bool start)
    {
        leia_core_init_configuration_set_face_tracking_start(_ptr, start);
    }
    void SetFaceTrackingCheckPermission(bool checkPermission)
    {
        leia_core_init_configuration_set_face_tracking_check_permission(_ptr, checkPermission);
    }
    void SetFaceTrackingPermissionDialogKillProcess(bool permissionDialogKillProcess)
    {
        leia_core_init_configuration_set_face_tracking_permission_dialog_kill_process(_ptr, permissionDialogKillProcess);
    }
    void SetFaceTrackingServerLogLevel(leia_log_level serverLogLevel)
    {
        leia_core_init_configuration_set_face_tracking_server_log_level(_ptr, serverLogLevel);
    }

    leia_core_init_configuration* GetHandle()
    {
        return _ptr;
    }

private:
    leia_core_init_configuration* _ptr;
};

/**
 * @brief C++ wrapper for leia_core
 */
class Core {
public:
    Core(CoreInitConfiguration& config) : _core(leia_core_init_async(config.GetHandle())), _isCoreOwned(true)
    {
#if !defined(LEIA_WRAPPER_DISABLE_EXCEPTION)
        if (!_core)
        {
            throw CoreInitFailed();
        }
#endif // !LEIA_WRAPPER_DISABLE_EXCEPTION
    }
    ~Core()
    {
        if (_isCoreOwned)
        {
            leia_core_release(_core);
        }
    }
    Core(Core const&) = delete;
    Core& operator=(Core const&) = delete;
    Core(Core&&) = delete;
    Core& operator=(Core&&) = delete;

#if defined(LEIA_WRAPPER_DISABLE_EXCEPTION)
    LEIA_NODISCARD
    bool IsValid() const
    {
        return _core != nullptr;
    }
#endif // !LEIA_WRAPPER_DISABLE_EXCEPTION

    LEIA_NODISCARD
    bool IsInitialized() const
    {
        return bool(leia_core_is_initialized(_core));
    }
    LEIA_NODISCARD
    bool IsValidationEnabled() const
    {
        return bool(leia_core_is_validation_enabled(_core));
    }

    LEIA_NODISCARD
    device::Config* GetDeviceConfig()
    {
        return leia_core_get_device_config(_core);
    }

    void SyncDeviceConfig(device::Config* config)
    {
        leia_core_sync_device_config(_core, config);
    }

    void ReleaseDeviceConfig(device::Config* config)
    {
        leia_core_release_device_config(_core, config);
    }

    LEIA_NODISCARD
    float GetCenterView()
    {
        return leia_core_get_center_view(_core);
    }
    void SetCenterView(float centerView)
    {
        leia_core_set_center_view(_core, centerView);
    }
    void OnResume()
    {
        leia_core_on_resume(_core);
    }
    void OnPause()
    {
        leia_core_on_pause(_core);
    }
    void SetBacklight(bool enable)
    {
        leia_core_set_backlight(_core, enable);
    }
    LEIA_NODISCARD
    bool GetBacklight()
    {
        return bool(leia_core_get_backlight(_core));
    }
    LEIA_NODISCARD
    bool IsOverlayDevice()
    {
        return bool(leia_core_is_overlay_device(_core));
    }
    bool EnableFaceTracking(bool enable)
    {
        return bool(leia_core_enable_face_tracking(_core, enable));
    }
    LEIA_NODISCARD
    bool IsFaceTrackingEnabled()
    {
        return bool(leia_core_is_face_tracking_enabled(_core));
    }
    void StartFaceTracking(bool start)
    {
        leia_core_start_face_tracking(_core, start);
    }
    LEIA_NODISCARD
    bool IsFaceTrackingStarted()
    {
        return bool(leia_core_is_face_tracking_started(_core));
    }
    void SetFaceTrackerConfiguration(bool trackLeft, bool trackRight)
    {
        leia_core_set_face_tracker_configuration(_core, trackLeft, trackRight);
    }
    void SetFaceTrackingRuntime(FaceTrackingRuntimeType runtime)
    {
        leia_core_set_face_tracking_runtime(_core, runtime);
    }
    LEIA_NODISCARD
    FaceTrackingRuntimeType GetFaceTrackingRuntime()
    {
        return leia_core_get_face_tracking_runtime(_core);
    }
    void SetFaceTrackingSharedCameraSink(SharedCameraSink* sink)
    {
        leia_core_set_face_tracking_shared_camera_sink(_core, sink);
    }
    LEIA_NODISCARD
    bool GetPrimaryFace(FloatSlice position)
    {
        return bool(leia_core_get_primary_face(_core, position));
    }
    LEIA_NODISCARD
    bool GetNonPredictedPrimaryFace(FloatSlice position)
    {
        return bool(leia_core_get_non_predicted_primary_face(_core, position));
    }
    void SetFaceTrackingFrameListener(OWNING(FaceTrackingFrameListener*) listener)
    {
        leia_core_set_face_tracking_frame_listener(_core, listener);
    }
    void SetFaceTrackingProfiling(bool enable)
    {
        leia_core_set_face_tracking_profiling(_core, enable);
    }
    void SetFaceTrackingCaptureLux(bool enable)
    {
        leia_core_set_face_tracking_capture_lux(_core, enable);
    }
    void EnableNoFaceMode(bool enable)
    {
        leia_core_enable_no_face_mode(_core, enable);
    }
    LEIA_NODISCARD
    AssetManager GetAssetManager()
    {
        return {leia_core_get_asset_manager(_core)};
    }

    LEIA_NODISCARD
    leia_core* GetHandle()
    {
        return _core;
    }
    Core(leia_core* core) : _core(core), _isCoreOwned(false)
    {
    }

private:
    leia_core* _core;
    bool       _isCoreOwned;
};

// using LeiaSDK = Core;
// using ILeiaSDK = Core;

LEIA_NODISCARD
inline bool IsFaceTrackingRuntimeSupported(FaceTrackingRuntimeType runtime)
{
    return bool(leia_is_face_tracking_runtime_supported(runtime));
}

LEIA_NODISCARD
inline bool IsLicenseRequired()
{
    return bool(leia_core_is_license_required());
}

} // namespace leia

/*!
 * @}
 */

#endif // CNSDK_LEIA_CORE_CXX_CORE_HPP
