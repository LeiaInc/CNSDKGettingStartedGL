#ifndef CNSDK_LEIA_CORE_CXX_INTERLACER_HPP
#define CNSDK_LEIA_CORE_CXX_INTERLACER_HPP

#include "leia/core/cxx/core.hpp"
#include "leia/core/interlacer.h"

/*!
 * @addtogroup interlacer
 * @{
 */

namespace leia {

using InterlacerDebugMenuConfiguration = leia_interlacer_debug_menu_configuration;
using ShaderDebugMode                  = leia_shader_debug_mode;
using FitMode                          = leia_fit_mode;
using TileLayout                       = leia_tile_layout;
using CalibrationPattern               = leia_calibration_pattern;
using InterlacerSingleViewModeListener = leia_interlacer_single_view_mode_listener;
using GraphicsAPI                      = leia_graphics_api;
using GuiInputState                    = leia_interlacer_gui_input_state;

/**
 * @brief C++ wrapper for leia_interlacer
 */
class Interlacer {
public:
    ~Interlacer()
    {
        if (_isInterlacerOwned)
        {
            leia_interlacer_release(_interlacer);
        }
    }
    Interlacer(Interlacer const&) = delete;
    Interlacer& operator=(Interlacer const&) = delete;
    Interlacer(Interlacer&&) = delete;
    Interlacer& operator=(Interlacer&&) = delete;

#if defined(LEIA_WRAPPER_DISABLE_EXCEPTION)
    bool IsValid() const
    {
        return _interlacer != nullptr;
    }
#endif // !LEIA_WRAPPER_DISABLE_EXCEPTION

    void EnableSharpening(bool enable)
    {
        leia_interlacer_enable_sharpening(_interlacer, enable);
    }
    LEIA_NODISCARD
    bool IsSharpeningEnabled() const
    {
        return bool(leia_interlacer_is_sharpening_enabled(_interlacer));
    }

    /**
     * @brief Sets the number of layers to interlace.
     *
     * Use this method to set the number of layers you want to be interlaced.
     *
     * @see GetLayerCount
     *
     * @param layerCount Number of layers.
     *
     * @public @memberof Interlacer
     */
    void SetLayerCount(int layerCount)
    {
        leia_interlacer_set_layer_count(_interlacer, layerCount);
    }

    /**
     * @brief Gets the current number of layers.
     *
     * @see SetLayerCount
     *
     * @return The number of layers that will be interlaced.
     *
     * @public @memberof Interlacer
     */
    LEIA_NODISCARD
    int GetLayerCount() const
    {
        return leia_interlacer_get_layer_count(_interlacer);
    }

    void SetShaderDebugMode(ShaderDebugMode mode)
    {
        leia_interlacer_set_shader_debug_mode(_interlacer, mode);
    }
    LEIA_NODISCARD
    ShaderDebugMode GetShaderDebugMode()
    {
        return leia_interlacer_get_shader_debug_mode(_interlacer);
    }

    void SetNumTiles(int32_t tx, int32_t ty)
    {
        leia_interlacer_set_num_tiles(_interlacer, tx, ty);
    }
    void GetNumTiles(int32_t* tx, int32_t* ty) const
    {
        leia_interlacer_get_num_tiles(_interlacer, tx, ty);
    }

    void SetTileLayout(TileLayout tileLayout)
    {
        leia_interlacer_set_tile_layout(_interlacer, tileLayout);
    }

    void SetOutputAsTiles(bool enable)
    {
        leia_interlacer_output_as_tiles(_interlacer, enable);
    }

    void SetBaselineScaling(float baseline)
    {
        leia_interlacer_set_baseline_scaling(_interlacer, baseline);
    }
    LEIA_NODISCARD
    float GetBaselineScaling()
    {
        return leia_interlacer_get_baseline_scaling(_interlacer);
    }
    LEIA_NODISCARD
    float GetConvergenceDistance()
    {
        return leia_interlacer_get_convergence_distance(_interlacer);
    }
    void SetConvergenceDistance(float distance)
    {
        leia_interlacer_set_convergence_distance(_interlacer, distance);
    }
    // TODO: Port to struct?
    void GetConvergedOrthographicViewInfo(int32_t viewIndex,
                                          LEIA_CONST_FLOAT_SLICE(3) cameraPos,
                                          LEIA_CONST_FLOAT_SLICE(3) cameraDir,
                                          LEIA_CONST_FLOAT_SLICE(3) cameraUp,
                                          float width,
                                          float height,
                                          float nearPlane,
                                          float farPlane,
                                          LEIA_FLOAT_SLICE(3) viewPos,
                                          LEIA_FLOAT_SLICE(16) viewProjectionMatrix,
                                          float* viewShearX,
                                          float* viewShearY)
    {
        leia_interlacer_get_converged_orthographic_view_info(
            _interlacer, viewIndex, cameraPos, cameraDir, cameraUp, width, height, nearPlane, farPlane, viewPos, viewProjectionMatrix, viewShearX, viewShearY);
    }
    // TODO: Port to struct?
    void GetConvergedPerspectiveViewInfo(int32_t viewIndex,
                                         LEIA_CONST_FLOAT_SLICE(3) cameraPos,
                                         LEIA_CONST_FLOAT_SLICE(3) cameraDir,
                                         LEIA_CONST_FLOAT_SLICE(3) cameraUp,
                                         float fieldOfView,
                                         float aspectRatio,
                                         float nearPlane,
                                         float farPlane,
                                         LEIA_FLOAT_SLICE(3) viewPos,
                                         LEIA_FLOAT_SLICE(16) viewProjectionMatrix,
                                         float* viewFieldOfView,
                                         float* viewShearX,
                                         float* viewShearY)
    {
        leia_interlacer_get_converged_perspective_view_info(_interlacer,
                                                            viewIndex,
                                                            cameraPos,
                                                            cameraDir,
                                                            cameraUp,
                                                            fieldOfView,
                                                            aspectRatio,
                                                            nearPlane,
                                                            farPlane,
                                                            viewPos,
                                                            viewProjectionMatrix,
                                                            viewFieldOfView,
                                                            viewShearX,
                                                            viewShearY);
    }
    void SetFitMode(FitMode mode)
    {
        leia_interlacer_set_fit_mode(_interlacer, mode);
    }
    LEIA_NODISCARD
    FitMode GetFitMode()
    {
        return leia_interlacer_get_fit_mode(_interlacer);
    }
    void GetRectMatrix(LEIA_FLOAT_SLICE(16) transformMatrix)
    {
        leia_interlacer_get_rect_matrix(_interlacer, transformMatrix);
    }
    void SetLocalMatrix(LEIA_CONST_FLOAT_SLICE(16) transformMatrix)
    {
        leia_interlacer_set_local_matrix(_interlacer, transformMatrix);
    }
    void SetCustomTextureMatrix(LEIA_CONST_FLOAT_SLICE(16) textureTransform)
    {
        leia_interlacer_set_custom_texture_matrix(_interlacer, textureTransform);
    }
    void EnableUserMatrix(bool enabled)
    {
        leia_interlacer_set_enable_user_matrix(_interlacer, enabled);
    }
    void GetUserMatrix(LEIA_FLOAT_SLICE(16) transformMatrix)
    {
        leia_interlacer_get_user_matrix(_interlacer, transformMatrix);
    }
    void SetUserMatrix(LEIA_CONST_FLOAT_SLICE(16) transformMatrix)
    {
        leia_interlacer_set_user_matrix(_interlacer, transformMatrix);
    }
    void SetAlpha(float alpha)
    {
        leia_interlacer_set_alpha(_interlacer, alpha);
    }
    void SetFlipInputUvVertical(bool flip)
    {
        leia_interlacer_set_flip_input_uv_vertical(_interlacer, leia_bool(flip));
    }
    void SetSingleViewMode(bool setSingleView, int32_t singleViewIndex)
    {
        leia_interlacer_set_single_view_mode(_interlacer, setSingleView, singleViewIndex);
    }
    void SetSingleViewModeListener(InterlacerSingleViewModeListener const* listener)
    {
        leia_interlacer_set_single_view_mode_listener(_interlacer, listener);
    }
    void SetSourceViewsSize(int32_t sourceWidth, int32_t sourceHeight, bool isHorizontalViews)
    {
        leia_interlacer_set_source_views_size(_interlacer, sourceWidth, sourceHeight, isHorizontalViews);
    }
    void GetSourceViewsSize(int32_t* sourceWidth, int32_t* sourceHeight, bool* isHorizontalViews)
    {
        leia_bool _isHorizontalViews;
        leia_interlacer_get_source_views_size(_interlacer, sourceWidth, sourceHeight, &_isHorizontalViews);
        *isHorizontalViews = bool(_isHorizontalViews);
    }
    void SetSourceViewsSRGB(bool isSRGB)
    {
        leia_interlacer_set_source_views_srgb(_interlacer, isSRGB);
    }
    void SetInShaderSRGBConversion(bool sRGBRead, bool sRGBWrite)
    {
        leia_interlacer_set_in_shader_srgb_conversion(_interlacer, sRGBRead, sRGBWrite);
    }
    void SetViewportScreenPosition(int32_t posX, int32_t posY)
    {
        leia_interlacer_set_viewport_screen_position(_interlacer, posX, posY);
    }
    void SetViewport(int32_t posX, int32_t posY, int32_t width, int32_t height)
    {
        leia_interlacer_set_viewport(_interlacer, posX, posY, width, height);
    }
    void EnableReconvergence(bool enable)
    {
        leia_interlacer_enable_reconvergence(_interlacer, enable);
    }
    LEIA_NODISCARD
    bool IsReconvergenceEnabled()
    {
        return bool(leia_interlacer_is_reconvergence_enabled(_interlacer));
    }

    /**
     * @brief Get the reconvergence amount.
     *
     * Use this method to retrieve the reconvergence amount set via SetReconvergence().
     *
     * @see IsReconvergenceEnabled, EnableReconvergence, SetReconvergence, SetReconvergenceZoom
     *
     * @param[in] layer Layer for the reconverergence (default is 0)
     *
     * @return The current reconvergence amount.
     *
     * @public @memberof Interlacer
     */
    LEIA_NODISCARD
    float GetReconvergence(int layer = 0)
    {
        return leia_interlacer_get_reconvergence(_interlacer, layer);
    }

    /**
     * @brief Set the reconvergence amount.
     *
     * Using reconvergence allows shifting the views to account for disparity.
     *
     * @see GetReconvergence, IsReconvergenceEnabled, EnableReconvergence, SetReconvergenceZoom
     *
     * @param[in] value Amount of reconvergence.
     * @param[in] layer Layer for the reconverergence (default is 0)
     *
     * @public @memberof Interlacer
     */
    void SetReconvergence(float value, int layer = 0)
    {
        leia_interlacer_set_reconvergence(_interlacer, value, layer);
    }

    /**
     * @brief Set what directions reconvergence is calculated for.
     *
     * Use this method to specify whether you wish to apply reconvergence horizontally or horizontally
     * and vertically.
     *
     * @see GetReconvergence, IsReconvergenceEnabled, EnableReconvergence, SetReconvergence
     *
     * @param[in] zoomX Whether to apply reconvergence in the horizontal direction.
     * @param[in] zoomXY Whether to apply reconvergence in the horizontal and vertical direction.
     * @param[in] layer Layer for the reconverergence (default is 0)
     *
     * @public @memberof Interlacer
     */
    void SetReconvergenceZoom(bool zoomX, bool zoomXY, int layer = 0)
    {
        leia_interlacer_set_reconvergence_zoom(_interlacer, zoomX, zoomXY, layer);
    }
    LEIA_NODISCARD
    int32_t GetNumViews()
    {
        return leia_interlacer_get_num_views(_interlacer);
    }
    void SetUseAtlasForViews(bool useAtlasForViews)
    {
        leia_interlacer_set_use_atlas_for_views(_interlacer, useAtlasForViews);
    }
    LEIA_NODISCARD
    bool GetUseAtlasForViews()
    {
        return bool(leia_interlacer_get_use_atlas_for_views(_interlacer));
    }
    void GetViewOffset(int32_t view, LEIA_FLOAT_SLICE(3) offset)
    {
        leia_interlacer_get_view_offset(_interlacer, view, offset);
    }
    void InitializeGui(InterlacerDebugMenuConfiguration const* config, bool sRGB)
    {
        leia_interlacer_initialize_gui(_interlacer, config, sRGB);
    }
    void SetGuiVisibility(bool isVisible)
    {
        leia_interlacer_set_gui_visibility(_interlacer, isVisible);
    }
    LEIA_NODISCARD
    bool IsGuiVisible()
    {
        return bool(leia_interlacer_is_gui_visible(_interlacer));
    }
#if defined(LEIA_OS_WINDOWS)
    GuiInputState ProcessGuiInput(void* surface, uint32_t msg, uint64_t wparam, int64_t lparam)
    {
        return leia_interlacer_process_gui_input_wndproc(_interlacer, surface, msg, wparam, lparam);
    }
#elif defined(LEIA_OS_ANDROID)
    GuiInputState ProcessGuiInput(AInputEvent const* inputEvent)
    {
        return leia_interlacer_process_gui_input_ndk(_interlacer, inputEvent);
    }
    GuiInputState ProcessGuiMotionInput(JNIEnv* jni, jobject motionInput)
    {
        return leia_interlacer_process_gui_motion_input(_interlacer, jni, motionInput);
    }
#endif // LEIA_OS_WINDOWS
    void ShutdownGui()
    {
        leia_interlacer_shutdown_gui(_interlacer);
    }
    void SetCalibrationPattern(CalibrationPattern pattern)
    {
        leia_interlacer_set_calibration_pattern(_interlacer, pattern);
    }
    void SetZonePlate(float posX, float posY, float scaleX, float scaleY)
    {
        leia_interlacer_set_zone_plate(_interlacer, posX, posY, scaleX, scaleY);
    }
    void SetGamma(float rGamma, float gGamma, float bGamma)
    {
        leia_interlacer_set_gamma(_interlacer, rGamma, gGamma, bGamma);
    }
    void SetAspectRatioOffset(float offset)
    {
        leia_interlacer_set_aspect_ratio_offset(_interlacer, offset);
    }
    LEIA_NODISCARD
    float GetAspectRatioOffset()
    {
        return leia_interlacer_get_aspect_ratio_offset(_interlacer);
    }
    void SetInvertScreenTextureCoordinates(bool invert)
    {
        leia_interlacer_set_invert_screen_texture_coordinates(_interlacer, invert);
    }

    LEIA_NODISCARD
    GraphicsAPI GetGraphicsAPI()
    {
        return leia_interlacer_get_graphics_api(_interlacer);
    }

    LEIA_NODISCARD
    leia_interlacer* GetHandle()
    {
        return _interlacer;
    }

protected:
    Interlacer(leia_interlacer* interlacer, bool isInterlacerOwned) : _interlacer(interlacer), _isInterlacerOwned(isInterlacerOwned)
    {
#if !defined(LEIA_WRAPPER_DISABLE_EXCEPTION)
        if (!_interlacer)
        {
            throw CoreInitFailed();
        }
#endif // !LEIA_WRAPPER_DISABLE_EXCEPTION
    }

protected:
    leia_interlacer* _interlacer;
    bool             _isInterlacerOwned;
};

/**
 * @brief C++ wrapper for leia_interlacer_init_configuration
 */
class InterlacerInitConfiguration {
public:
    InterlacerInitConfiguration() : _ptr(leia_interlacer_init_configuration_alloc())
    {
    }
    ~InterlacerInitConfiguration()
    {
        leia_interlacer_init_configuration_free(_ptr);
    }
    void SetIsProtected(bool isProtected)
    {
        leia_interlacer_init_configuration_set_is_protected(_ptr, isProtected);
    }
    void SetUseAtlasForViews(bool useAtlasForViews)
    {
        leia_interlacer_init_configuration_set_use_atlas_for_views(_ptr, useAtlasForViews);
    }
    LEIA_NODISCARD
    leia_interlacer_init_configuration* GetHandle() const
    {
        return _ptr;
    }

private:
    leia_interlacer_init_configuration* _ptr;
};

using ViewInfoMode = leia_view_info_mode;

LEIA_FORCE_INLINE
void GetViewComfortZone(ViewInfoMode mode,
                        float        baselineScaling,
                        float        convergencePlaneDistance,
                        float        nearPlane,
                        float        farPlane,
                        float*       viewComfortZoneNear,
                        float*       viewComfortZoneFar)
{
    leia_get_view_comfort_zone(mode, baselineScaling, convergencePlaneDistance, nearPlane, farPlane, viewComfortZoneNear, viewComfortZoneFar);
}

LEIA_FORCE_INLINE
void GetViewInfo(ViewInfoMode mode,
                 LEIA_CONST_FLOAT_SLICE(3) viewOffset,
                 LEIA_CONST_FLOAT_SLICE(3) cameraPos,
                 LEIA_CONST_FLOAT_SLICE(3) cameraDir,
                 LEIA_CONST_FLOAT_SLICE(3) cameraUp,
                 float perspectiveFieldOfView,
                 float perspectiveAspectRatio,
                 float orthoWidth,
                 float orthoHeight,
                 float nearPlane,
                 float farPlane,
                 float convergencePlaneDistance,
                 float convergencePlaneHeight,
                 LEIA_FLOAT_SLICE(3) viewPos,
                 LEIA_FLOAT_SLICE(16) viewProjectionMatrix,
                 float* viewFieldOfView,
                 float* viewShearX,
                 float* viewShearY)
{
    leia_get_view_info(mode,
                       viewOffset,
                       cameraPos,
                       cameraDir,
                       cameraUp,
                       perspectiveFieldOfView,
                       perspectiveAspectRatio,
                       orthoWidth,
                       orthoHeight,
                       nearPlane,
                       farPlane,
                       convergencePlaneDistance,
                       convergencePlaneHeight,
                       viewPos,
                       viewProjectionMatrix,
                       viewFieldOfView,
                       viewShearX,
                       viewShearY);
}

} // namespace leia

/*!
 * @}
 */

#endif // CNSDK_LEIA_CORE_CXX_INTERLACER_HPP
