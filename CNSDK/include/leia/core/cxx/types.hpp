#ifndef CNSDK_LEIA_CORE_CXX_TYPES_HPP
#define CNSDK_LEIA_CORE_CXX_TYPES_HPP

#include "leia/core/types.h"

namespace leia {

using FaceTrackingRuntimeType = leia_face_tracking_runtime_type;

using FitMode            = leia_fit_mode;
using GraphicsApi        = leia_graphics_api;
using ShaderDebugMode    = leia_shader_debug_mode;
using TileLayout         = leia_tile_layout;
using CalibrationPattern = leia_calibration_pattern;

using InterlacerGuiInputState          = leia_interlacer_gui_input_state;
using InterlacerGuiSurface             = leia_interlacer_gui_surface;
using InterlacerGuiConfiguration       = leia_interlacer_gui_configuration;
using InterlacerDebugMenuConfiguration = leia_interlacer_debug_menu_configuration;

LEIA_NODISCARD
LEIA_FORCE_INLINE const char* ToUiStr(FaceTrackingRuntimeType faceTrackingRuntimeType)
{
    return leia_face_tracking_runtime_type_to_ui_str(faceTrackingRuntimeType);
}

LEIA_NODISCARD
LEIA_FORCE_INLINE const char* ToUiStr(ShaderDebugMode shaderDebugMode)
{
    return leia_shader_debug_mode_to_ui_str(shaderDebugMode);
}

} // namespace leia

#endif // CNSDK_LEIA_CORE_CXX_TYPES_HPP
