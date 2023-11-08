#pragma once

#include "leia/common/api.h"
#include "leia/common/types.h"

#include <string_view>

namespace leia {

using Timestamp             = leia_timestamp;
using TimestampSpace        = leia_timestamp_space;
using ImageDesc             = leia_image_desc;
using CameraIntrinsics      = leia_camera_intrinsics;
using FaceDetectorBackend   = leia_face_detector_backend;
using FaceDetectorInputType = leia_face_detector_input_type;
using FaceDetectorConfig    = leia_face_detector_config;
using SourceLocation        = leia_source_location;
using Orientation           = leia_orientation;

LEIA_NODISCARD
LEIA_FORCE_INLINE const char* ToStr(FaceDetectorBackend backend)
{
    return leia_face_detector_backend_to_str(backend);
}
LEIA_NODISCARD
LEIA_FORCE_INLINE const char* ToUiStr(FaceDetectorBackend backend)
{
    return leia_face_detector_backend_to_ui_str(backend);
}
LEIA_NODISCARD
LEIA_FORCE_INLINE const char* ToStr(FaceDetectorInputType inputType)
{
    return leia_face_detector_input_type_to_str(inputType);
}
LEIA_NODISCARD
LEIA_FORCE_INLINE const char* ToUiStr(FaceDetectorInputType inputType)
{
    return leia_face_detector_input_type_to_ui_str(inputType);
}
LEIA_NODISCARD
LEIA_FORCE_INLINE const char* ToUiStr(Orientation orientation)
{
    return leia_orientation_to_ui_str(orientation);
}

inline bool FromStr(std::string_view const& str, Orientation* out)
{
    if (str == "Landscape")
    {
        *out = LEIA_ORIENTATION_LANDSCAPE;
    }
    else if (str == "Portrait")
    {
        *out = LEIA_ORIENTATION_PORTRAIT;
    }
    else if (str == "ReverseLandscape")
    {
        *out = LEIA_ORIENTATION_REVERSE_LANDSCAPE;
    }
    else if (str == "ReversePortrait")
    {
        *out = LEIA_ORIENTATION_REVERSE_PORTRAIT;
    }
    else
    {
        return false;
    }
    return true;
}

inline constexpr bool IsValid(Orientation orientation)
{
    return orientation >= LEIA_ORIENTATION_LANDSCAPE && orientation <= LEIA_ORIENTATION_REVERSE_PORTRAIT;
}

inline constexpr int32_t GetRelativeClockwiseAngle(Orientation from, Orientation to)
{
    if (!IsValid(from) || !IsValid(to))
    {
        return 0;
    }

    int32_t numSteps = 0;
    int32_t it       = int32_t(from);
    int32_t end      = int32_t(to);
    while (it != end)
    {
        it = (it + 1) % LEIA_ORIENTATION_COUNT;
        numSteps++;
    }
    return numSteps * 90;
}

static_assert(GetRelativeClockwiseAngle(LEIA_ORIENTATION_LANDSCAPE, LEIA_ORIENTATION_LANDSCAPE) == 0);
static_assert(GetRelativeClockwiseAngle(LEIA_ORIENTATION_LANDSCAPE, LEIA_ORIENTATION_PORTRAIT) == 90);
static_assert(GetRelativeClockwiseAngle(LEIA_ORIENTATION_LANDSCAPE, LEIA_ORIENTATION_REVERSE_PORTRAIT) == 270);
static_assert(GetRelativeClockwiseAngle(LEIA_ORIENTATION_REVERSE_LANDSCAPE, LEIA_ORIENTATION_LANDSCAPE) == 180);

} // namespace leia
