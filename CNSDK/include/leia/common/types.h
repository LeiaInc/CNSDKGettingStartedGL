#pragma once

#include "leia/common/slice.h"
#include "leia/common/log.h"

#include <stdint.h>

BEGIN_CAPI_DECL

typedef int32_t leia_bool;

typedef void (*leia_free_user_data_cb)(void* userData);

#pragma pack(push, 8)

enum leia_timestamp_space {
    /// Timestamp measures the time since an unspecified starting point
    /// and is monotonically increasing.
    LEIA_TIMESTAMP_SPACE_UNKNOWN = -1,

    /// Timestamp measures the time in the same timebase as system clock.
    /// Different cameras match different time points:
    ///   * realsense - the middle of exposure
    ///   * android - the start of exposure
    /// Depending on platform different system clock is used:
    ///   * windows - std::chrono::system_clock::now
    ///   * android - android.os.SystemClock.elapsedRealtime
    LEIA_TIMESTAMP_SPACE_SYSTEM = 0,

    _LEIA_TIMESTAMP_SPACE_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

struct leia_timestamp {
    double                    ms;
    enum leia_timestamp_space space;
};

struct leia_image_desc {
    uint8_t const* data;
    int32_t        width;
    int32_t        height;
    int32_t        rotation; // TODO: remove
};

struct leia_camera_intrinsics {
    int32_t   width; /* Width of the image in pixels */
    int32_t   height; /* Height of the image in pixels */
    float     ppx; /* Horizontal coordinate of the principal point of the image, as a pixel offset from the left edge */
    float     ppy; /* Vertical coordinate of the principal point of the image, as a pixel offset from the top edge */
    float     fx; /* Focal length of the image plane, as a multiple of pixel width */
    float     fy; /* Focal length of the image plane, as a multiple of pixel height */
    double    distortionCoeffs[8]; /* Distortion coefficients */
    leia_bool isMirrored; /* Whether the image is mirrored or not */
};

struct leia_normalized_camera_intrinsics {
    float fx; /* Horizontal normalized coordinate of the principal point of the image, as an offset from the left edge */
    float fy; /* Vertical normalized coordinate of the principal point of the image, as an offset from the top edge */
    float ppx; /* Horizontal focal length of the image plane normalized by the width */
    float ppy; /* Vertical focal length of the image plane normalized by the height */
    float distortionCoeffs[8]; /* Distortion coefficients, OpenCV-style */
};

struct leia_vector2d {
    double x;
    double y;
};

struct leia_vector3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
    };
};

struct leia_vector4 {
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        float v[4];
    };
};

struct leia_mat4 {
    union {
        struct leia_vector4 col[4];
        float               m[16];
    };
};

enum leia_orientation {
    LEIA_ORIENTATION_UNSPECIFIED       = -1,
    LEIA_ORIENTATION_LANDSCAPE         = 0,
    LEIA_ORIENTATION_PORTRAIT          = 1,
    LEIA_ORIENTATION_REVERSE_LANDSCAPE = 2,
    LEIA_ORIENTATION_REVERSE_PORTRAIT  = 3,
    LEIA_ORIENTATION_COUNT             = 4,

    _LEIA_ORIENTATION_MAKE_ENUM_32BIT = 0x7FFFFFFF
};

enum leia_face_detector_backend {
    kLeiaFaceDetectorBackendUnknown = 0,
    kLeiaFaceDetectorBackendCPU     = 1 << 0,
    kLeiaFaceDetectorBackendGPU     = 1 << 1,

    kNumLeiaFaceDetectorBackends = 2,
};

enum leia_face_detector_input_type {
    kLeiaFaceDetectorInputTypeUnknown = 0,
    kLeiaFaceDetectorInputTypeCPU     = 1 << 0,
    kLeiaFaceDetectorInputTypeGPU     = 1 << 1,

    kNumLeiaFaceDetectorInputTypes = 2,
};

struct leia_face_detector_config {
    enum leia_face_detector_backend    backend;
    enum leia_face_detector_input_type inputType;
};

struct leia_source_location {
    const char* filename;
    const char* funcname;
    int         line;
};

#define LEIA_SOURCE_LOCATION              \
    leia_source_location                  \
    {                                     \
        __FILE__, LEIA_FUNCTION, __LINE__ \
    }

LEIA_NODISCARD
inline const char* leia_face_detector_backend_to_str(enum leia_face_detector_backend backend)
{
    switch (backend)
    {
        case kLeiaFaceDetectorBackendCPU:
            return "kLeiaFaceDetectorBackendCPU";
        case kLeiaFaceDetectorBackendGPU:
            return "kLeiaFaceDetectorBackendGPU";
        default:
            return "kLeiaFaceDetectorBackendUnknown";
    }
}

LEIA_NODISCARD
inline const char* leia_face_detector_backend_to_ui_str(enum leia_face_detector_backend backend)
{
    switch (backend)
    {
        case kLeiaFaceDetectorBackendCPU:
            return "CPU";
        case kLeiaFaceDetectorBackendGPU:
            return "GPU";
        default:
            return "Unknown";
    }
}

LEIA_NODISCARD
inline const char* leia_face_detector_input_type_to_str(enum leia_face_detector_input_type inputType)
{
    switch (inputType)
    {
        case kLeiaFaceDetectorInputTypeCPU:
            return "kLeiaFaceDetectorInputTypeCPU";
        case kLeiaFaceDetectorInputTypeGPU:
            return "kLeiaFaceDetectorInputTypeGPU";
        default:
            return "kLeiaFaceDetectorInputTypeUnknown";
    }
}

LEIA_NODISCARD
inline const char* leia_face_detector_input_type_to_ui_str(enum leia_face_detector_input_type inputType)
{
    switch (inputType)
    {
        case kLeiaFaceDetectorInputTypeCPU:
            return "CPU";
        case kLeiaFaceDetectorInputTypeGPU:
            return "GPU";
        default:
            return "Unknown";
    }
}

LEIA_NODISCARD
inline const char* leia_orientation_to_ui_str(enum leia_orientation orientation)
{
    switch (orientation)
    {
        case LEIA_ORIENTATION_LANDSCAPE:
            return "Landscape";
        case LEIA_ORIENTATION_PORTRAIT:
            return "Portrait";
        case LEIA_ORIENTATION_REVERSE_LANDSCAPE:
            return "ReverseLandscape";
        case LEIA_ORIENTATION_REVERSE_PORTRAIT:
            return "ReversePortrait";
        default:
            return "Unknown";
    }
}

LEIA_NODISCARD
inline struct leia_float_slice leia_vector3_to_slice(struct leia_vector3* v)
{
    struct leia_float_slice ret;
    ret.data   = v->v;
    ret.length = 3;
    return ret;
}
LEIA_NODISCARD
inline struct leia_float_slice leia_vector4_to_slice(struct leia_vector4* v)
{
    struct leia_float_slice ret;
    ret.data   = v->v;
    ret.length = 4;
    return ret;
}
LEIA_NODISCARD
inline struct leia_float_slice leia_mat4_to_slice(struct leia_mat4* v)
{
    struct leia_float_slice ret;
    ret.data   = v->m;
    ret.length = 16;
    return ret;
}

#pragma pack(pop)

END_CAPI_DECL
