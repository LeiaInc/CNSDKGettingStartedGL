/*
 * Copyright 2023 (c) Leia Inc.  All rights reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Leia Inc. and its suppliers, if any.  The
 * intellectual and technical concepts contained herein are
 * proprietary to Leia Inc. and its suppliers and may be covered
 * by U.S. and Foreign Patents, patents in process, and are
 * protected by trade secret or copyright law.  Dissemination of
 * this information or reproduction of this materials strictly
 * forbidden unless prior written permission is obtained from
 * Leia Inc.
 */
#pragma once

#include "leia/headTracking/common/api.h"
#include "leia/headTracking/common/types.h"

BEGIN_CAPI_DECL

#pragma pack(push, 4)

struct leia_headtracking_raw_faces {
    int                               numFaces;
    struct leia_headtracking_raw_face faces[LEIA_HEADTRACKING_MAX_NUM_FACES];
};

struct leia_headtracking_detected_faces {
    int                                    numFaces;
    struct leia_headtracking_detected_face faces[LEIA_HEADTRACKING_MAX_NUM_FACES];
};

// All timestamps are in the system clock space. See Platform::GetSystemTimeNs.
struct leia_headtracking_frame_profiling {
    int64_t cameraExposureTime;
    // Face detector starts processing a camera frame
    int64_t faceDetectorStartTime;
    // Face detector ends processing a camera frame
    int64_t faceDetectorEndTime;
    // Engine sends frame to the user
    int64_t apiTimestamp;
};

#pragma pack(pop)

LHT_COMMON_API
leia_headtracking_status leia_headtracking_frame_get_tracking_result(struct leia_headtracking_frame*, struct leia_headtracking_tracking_result*);

LHT_COMMON_API
leia_headtracking_status leia_headtracking_frame_get_raw_faces(struct leia_headtracking_frame*, struct leia_headtracking_raw_faces*);

LHT_COMMON_API
leia_headtracking_status leia_headtracking_frame_get_detected_faces(struct leia_headtracking_frame*, struct leia_headtracking_detected_faces*);

LHT_COMMON_API
leia_headtracking_status leia_headtracking_frame_get_profiling(struct leia_headtracking_frame*, struct leia_headtracking_frame_profiling*);

/**
 * @brief Get current camera-detected lux value if available.
 */
LHT_COMMON_API
float leia_headtracking_frame_get_lux(struct leia_headtracking_frame*);

LHT_COMMON_API
void leia_headtracking_frame_release(struct leia_headtracking_frame*);

/**
 * @brief Get the device acceleration.
 *
 * It's an experimental API, see leia_get_experimental_api() for more info.
 * @public @memberof leia_headtracking_frame
 */
typedef leia_bool (*leia_headtracking_frame_get_device_acceleration)(struct leia_headtracking_frame*, struct leia_vector3*);

/**
 * @brief Get the device gyroscope.
 *
 * By default, gyroscope device is not active. To activate, set CNSDK_SENSOR_MANAGER_GYROSCOPE environment variable.
 *
 * It's an experimental API, see leia_get_experimental_api() for more info.
 * @public @memberof leia_headtracking_frame
 */
typedef leia_bool (*leia_headtracking_frame_get_device_gyroscope)(struct leia_headtracking_frame*, struct leia_vector3*);

//

END_CAPI_DECL
