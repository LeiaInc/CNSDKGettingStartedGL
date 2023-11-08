#pragma once

#include "leia/common/types.h"

BEGIN_CAPI_DECL

typedef int32_t leia_headtracking_status;

enum // leia_headtracking_status
{
    kLeiaHeadTrackingStatusSuccess = 0,
    kLeiaHeadTrackingStatusErrorInvalidInstance,
    kLeiaHeadTrackingStatusErrorUnknown,
};

#pragma pack(push, 4)

struct leia_headtracking_moving_point {
    struct leia_vector3 pos;
    struct leia_vector3 vel;
};

typedef uint32_t leia_headtracking_face_idx;

enum leia_headtracking_eye_idx {
    kLeiaHeadTrackingRightEyeIdx = 0,
    kLeiaHeadTrackingLeftEyeIdx,
    kLeiaHeadTrackingNumEyes
};

struct leia_headtracking_face {
    // Tracking point - 3D coordinate with camera transform and Kalman filter applied.
    struct leia_headtracking_moving_point point;
    // Pose angle
    struct leia_vector3                   angle;
    // // Index into the raw faces array.
    leia_headtracking_face_idx            rawFaceIndex;
};

struct leia_headtracking_raw_face {
    // Deprojected camera-space eye position.
    struct leia_vector3        eyePoints[kLeiaHeadTrackingNumEyes];
    // The point of tracking, see Configuration::trackRightEye/Configuration::trackLeftEye.
    struct leia_vector3        trackingPoint;
    // Index into FaceDetector::Output::faces.
    leia_headtracking_face_idx detectedFaceIndex;
};

struct leia_headtracking_detected_face_eye {
    // 2D coordinate on camera frame image plane (top-left origin)
    struct leia_vector2d imageCoord;
    // Z component in camera-space (projected distance)
    float                depth;
};

struct leia_headtracking_detected_face {
    struct leia_headtracking_detected_face_eye eyes[kLeiaHeadTrackingNumEyes];

    // Head location in mm. The origin point is the location of the camera.
    struct leia_vector3 posePosition;
    // Head rotation in radians. The rotation is a left handed coordinate system.
    struct leia_vector3 poseAngle;
    uint32_t            id;
};

#define LEIA_HEADTRACKING_MAX_NUM_FACES 3

struct leia_headtracking_tracking_result {
    int32_t                       num_faces;
    struct leia_headtracking_face faces[LEIA_HEADTRACKING_MAX_NUM_FACES];

    struct leia_timestamp timestamp;

    int32_t jumpFlag;
};

struct leia_headtracking_frame;

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

END_CAPI_DECL
