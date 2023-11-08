// The goal of these asserts to make sure data ABI requirements are met.
// This file is included in public headers.
// If you are reading this because one of the asserts fail,
// and you are the user of CNSDK, then I have bad news for you:
// your compiler lays out data structures in an unexpected manner.
// You will have to find out why, feel free to contact Leia support for help.
//
// But if you are a CNSDK developer, then you most likely have changed one of the structures.
// In this case, remember these simple rules:
//   1. Existing fields can only be renamed (in case you want to deprecate it) -> rename the corresponding offsetof assert.
// The asserts can be updated manually or by relying on the DumpAbiVerfication function.

#ifndef LEIA_ABI_SIZEOF_ASSERT
#    define LEIA_ABI_SIZEOF_ASSERT(...)
#endif
#ifndef LEIA_ABI_OFFSETOF_ASSERT
#    define LEIA_ABI_OFFSETOF_ASSERT(...)
#endif
#ifndef LEIA_ABI_TRIVIALLY_COPYABLE_ASSERT
#    define LEIA_ABI_TRIVIALLY_COPYABLE_ASSERT(...)
#endif

LEIA_ABI_OFFSETOF_ASSERT(struct leia_image_desc, data, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_image_desc, width, 8);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_image_desc, height, 12);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_image_desc, rotation, 16);
LEIA_ABI_SIZEOF_ASSERT(struct leia_image_desc, 24);

LEIA_ABI_OFFSETOF_ASSERT(struct leia_camera_intrinsics, width, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_camera_intrinsics, height, 4);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_camera_intrinsics, ppx, 8);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_camera_intrinsics, ppy, 12);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_camera_intrinsics, fx, 16);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_camera_intrinsics, fy, 20);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_camera_intrinsics, distortionCoeffs, 24);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_camera_intrinsics, isMirrored, 88);
LEIA_ABI_SIZEOF_ASSERT(struct leia_camera_intrinsics, 96);

LEIA_ABI_OFFSETOF_ASSERT(struct leia_timestamp, ms, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_timestamp, space, 8);
LEIA_ABI_SIZEOF_ASSERT(struct leia_timestamp, 16);

LEIA_ABI_OFFSETOF_ASSERT(struct leia_vector3, x, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_vector3, y, 4);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_vector3, z, 8);
LEIA_ABI_SIZEOF_ASSERT(struct leia_vector3, 12);

LEIA_ABI_SIZEOF_ASSERT(struct leia_mat4, 64);

LEIA_ABI_OFFSETOF_ASSERT(struct leia_event_log, message, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_event_log, level, 8);
LEIA_ABI_SIZEOF_ASSERT(struct leia_event_log, 16);

LEIA_ABI_OFFSETOF_ASSERT(struct leia_event_error, message, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_event_error, code, 8);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_event_error, component, 16);
// LEIA_ABI_SIZEOF_ASSERT(struct leia_event_error, 16); - Allowed to grow

LEIA_ABI_OFFSETOF_ASSERT(struct leia_event_component, id, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_event_component, code, 4);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_event_component, payload, 8);

LEIA_ABI_OFFSETOF_ASSERT(struct leia_event, type, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_event, v, 8);
// LEIA_ABI_SIZEOF_ASSERT(struct leia_event, 0); - Allowed to grow

LEIA_ABI_TRIVIALLY_COPYABLE_ASSERT(leia_headtracking_frame_profiling);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_headtracking_frame_profiling, cameraExposureTime, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_headtracking_frame_profiling, faceDetectorStartTime, 8);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_headtracking_frame_profiling, faceDetectorEndTime, 16);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_headtracking_frame_profiling, apiTimestamp, 24);
// LEIA_ABI_SIZEOF_ASSERT(struct leia_headtracking_frame_profiling, 0); - Allowed to grow

LEIA_ABI_OFFSETOF_ASSERT(struct leia_float_slice, data, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_float_slice, length, 8);
LEIA_ABI_SIZEOF_ASSERT(struct leia_float_slice, 16);

LEIA_ABI_OFFSETOF_ASSERT(struct leia_const_float_slice, data, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_const_float_slice, length, 8);
LEIA_ABI_SIZEOF_ASSERT(struct leia_const_float_slice, 16);

LEIA_ABI_OFFSETOF_ASSERT(struct leia_slice, data, 0);
LEIA_ABI_OFFSETOF_ASSERT(struct leia_slice, length, 8);
LEIA_ABI_SIZEOF_ASSERT(struct leia_slice, 16);

LEIA_ABI_SIZEOF_ASSERT(leia_log_level, 4);
LEIA_ABI_SIZEOF_ASSERT(enum leia_face_tracking_runtime_type, 4);
LEIA_ABI_SIZEOF_ASSERT(enum leia_timestamp_space, 4);
LEIA_ABI_SIZEOF_ASSERT(enum leia_face_detector_backend, 4);
LEIA_ABI_SIZEOF_ASSERT(enum leia_shader_debug_mode, 4);
LEIA_ABI_SIZEOF_ASSERT(enum leia_fit_mode, 4);
LEIA_ABI_SIZEOF_ASSERT(enum leia_core_event_code, 4);

#undef LEIA_ABI_SIZEOF_ASSERT
#undef LEIA_ABI_OFFSETOF_ASSERT
#undef LEIA_ABI_TRIVIALLY_COPYABLE_ASSERT
