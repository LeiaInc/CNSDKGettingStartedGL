#ifndef CNSDK_LEIA_CORE_TYPES_H
#define CNSDK_LEIA_CORE_TYPES_H

#include "leia/core/api.h"
#include "leia/core/fwd.h"
#include "leia/common/types.h"
#include "leia/common/jniTypes.h"

BEGIN_CAPI_DECL

#pragma pack(push, 8)

enum leia_graphics_api {
    LEIA_GRAPHICS_API_OPENGL = 0,
    LEIA_GRAPHICS_API_D3D11  = 1,
    LEIA_GRAPHICS_API_D3D12  = 2,
    LEIA_GRAPHICS_API_VULKAN = 3,

    _LEIA_GRAPHICS_API_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

struct leia_interlacer_gui_input_state {
    leia_bool wantCaptureInput; ///< if true, do not process any input
    leia_bool wantCaptureMouse; ///< if true, do not process mouse input
    leia_bool wantCaptureKeyboard; ///< if true, do not process keyboard input
    leia_bool wantTextInput; ///< if true, open textual keyboard input
};

/// HWND on Windows
typedef void* leia_interlacer_gui_surface;

struct leia_interlacer_gui_configuration {
    leia_interlacer_gui_surface surface;

    enum leia_graphics_api graphicsAPI;

    float fontGlobalScale;

    union {
        struct {
            void* device;
            void* deviceContext;
        } d3d11;
        struct {
            void*    device;
            int      numFramesInFlight;
            int      rtvFormat;
            void*    deviceCbvSrvHeap;
            uint64_t fontSrvCpuDescHandle;
            uint64_t fontSrvGpuDescHandle;
            void*    commandList;
        } d3d12;
        struct {
            void*    device;
            void*    descriptorPool;
            uint32_t imageCount;
            void*    instance;
            uint32_t minImageCount;
            int      mSAASamples;
            void*    physicalDevice;
            void*    pipelineCache;
            void*    queue;
            uint32_t queueFamily;
            uint32_t subpass;
            void*    renderPass;
            void*    commandBuffer;
            void*    commandPool;
        } vulkan;
    };
};

struct ImGuiContext;

struct leia_interlacer_debug_menu_configuration {
    struct leia_interlacer_gui_configuration gui;

    void (*customGui)(void* userData, struct ImGuiContext*);
    void* userData;
};

enum leia_face_tracking_runtime_type {
    LEIA_FACE_TRACKING_RUNTIME_IN_SERVICE = 0,
    LEIA_FACE_TRACKING_RUNTIME_IN_APP     = 1,

    _LEIA_FACE_TRACKING_RUNTIME_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

LEIA_NODISCARD
inline const char* leia_face_tracking_runtime_type_to_ui_str(enum leia_face_tracking_runtime_type runtime)
{
    switch (runtime)
    {
        case LEIA_FACE_TRACKING_RUNTIME_IN_APP:
            return "In-App";
        case LEIA_FACE_TRACKING_RUNTIME_IN_SERVICE:
            return "In-Service";
        default:
            return "Unknown";
    }
}

enum leia_shader_debug_mode {
    LEIA_SHADER_DEBUG_MODE_NONE = 0, ///< Debugging off
    LEIA_SHADER_DEBUG_MODE_SHOW_TEXTURE_COORDINATES, ///< Show texture coordinates
    LEIA_SHADER_DEBUG_MODE_SHOW_ALL_VIEWS, ///< Show all views (tiled, horizontal, or vertical)
    LEIA_SHADER_DEBUG_MODE_SHOW_FIRST_VIEW, ///< Show single first view
    LEIA_SHADER_DEBUG_MODE_SHOW_CALIBRATION_IMAGE, ///< Show calibration image
    LEIA_SHADER_DEBUG_MODE_SHOW_BEER_GLASS, ///< Show beer glass image
    LEIA_SHADER_DEBUG_MODE_SHOW_STEREO_IMAGE, ///< Show stereo image
    LEIA_SHADER_DEBUG_MODE_SHOW_STEREO_VIDEO, ///< Video Views

    LEIA_SHADER_DEBUG_MODE_COUNT,
    _LEIA_SHADER_DEBUG_MODE_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

LEIA_NODISCARD
inline const char* leia_shader_debug_mode_to_ui_str(enum leia_shader_debug_mode mode)
{
    switch (mode)
    {
        case LEIA_SHADER_DEBUG_MODE_NONE:
            return "None";
        case LEIA_SHADER_DEBUG_MODE_SHOW_TEXTURE_COORDINATES:
            return "Texture Coordinates";
        case LEIA_SHADER_DEBUG_MODE_SHOW_ALL_VIEWS:
            return "All Views";
        case LEIA_SHADER_DEBUG_MODE_SHOW_FIRST_VIEW:
            return "First View";
        case LEIA_SHADER_DEBUG_MODE_SHOW_CALIBRATION_IMAGE:
            return "Calibration Image";
        case LEIA_SHADER_DEBUG_MODE_SHOW_BEER_GLASS:
            return "Beer Glass";
        case LEIA_SHADER_DEBUG_MODE_SHOW_STEREO_IMAGE:
            return "Stereo Image";
        case LEIA_SHADER_DEBUG_MODE_SHOW_STEREO_VIDEO:
            return "Stereo Video";
        default:
            return "Unknown";
    }
}

enum leia_fit_mode {
    LEIA_FIT_MODE_FILL            = 0,
    LEIA_FIT_MODE_FIT_CENTER      = 1,
    LEIA_FIT_MODE_CROP_FILL       = 2,
    LEIA_FIT_MODE_CROP_FIT_SQUARE = 3,

    _LEIA_FIT_MODE_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

enum leia_calibration_pattern {
    LEIA_CALIBRATION_PATTERN_NONE                    = 0,
    LEIA_CALIBRATION_PATTERN_ZONE_PLATE              = 1,
    LEIA_CALIBRATION_PATTERN_THETA_LINES_RGB         = 2,
    LEIA_CALIBRATION_PATTERN_CENTER_VIEW_CIRCLE_BARS = 3,
    LEIA_CALIBRATION_PATTERN_D_OVER_N_BARS           = 4,
    LEIA_CALIBRATION_PATTERN_COUNT,

    // Not currently in use, but retaining for future

    LEIA_CALIBRATION_PATTERN_GAMMA_R_BOX         = 6,
    LEIA_CALIBRATION_PATTERN_GAMMA_G_BOX         = 7,
    LEIA_CALIBRATION_PATTERN_GAMMA_B_BOX         = 8,
    LEIA_CALIBRATION_PATTERN_R0_RED              = 9,
    LEIA_CALIBRATION_PATTERN_R0_WHITE            = 10,
    LEIA_CALIBRATION_PATTERN_RAINBOW_UNCORRECTED = 11,
    LEIA_CALIBRATION_PATTERN_RAINBOW_CORRECTED   = 12,

    LEIA_CALIBRATION_PATTERN_IMAGE_UNCORRECTED = 13,
    LEIA_CALIBRATION_PATTERN_IMAGE_CORRECTED   = 14,
    _LEIA_CALIBRATION_PATTERN_MAKE_ENUM_32BIT  = 0x7FFFFFFF
};

enum leia_tile_layout {
    LEIA_TILE_LAYOUT_LEFT_TO_RIGHT_DOWN_ROW_MAJOR    = 0,
    LEIA_TILE_LAYOUT_LEFT_TO_RIGHT_UP_ROW_MAJOR      = 1,
    LEIA_TILE_LAYOUT_RIGHT_TO_LEFT_DOWN_ROW_MAJOR    = 2,
    LEIA_TILE_LAYOUT_RIGHT_TO_LEFT_UP_ROW_MAJOR      = 3,
    LEIA_TILE_LAYOUT_LEFT_TO_RIGHT_DOWN_COLUMN_MAJOR = 4,
    LEIA_TILE_LAYOUT_RIGHT_TO_LEFT_DOWN_COLUMN_MAJOR = 5,
    LEIA_TILE_LAYOUT_LEFT_TO_RIGHT_UP_COLUMN_MAJOR   = 6,
    LEIA_TILE_LAYOUT_RIGHT_TO_LEFT_UP_COLUMN_MAJOR   = 7,

    _LEIA_TILE_LAYOUT_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

enum leia_view_info_mode {
    LEIA_VIEW_INFO_MODE_PERSPECTIVE            = 0,
    LEIA_VIEW_INFO_MODE_PERSPECTIVE_FROM_PLANE = 1,
    LEIA_VIEW_INFO_MODE_ORTHOGRAPHIC           = 2,

    _LEIA_VIEW_INFO_MODE_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

#pragma pack(pop)

END_CAPI_DECL

#endif // CNSDK_LEIA_CORE_TYPES_H
