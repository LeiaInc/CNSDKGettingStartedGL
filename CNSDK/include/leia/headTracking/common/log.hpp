#pragma once

#include "leia/common/log.hpp"
#include "leia/headTracking/common/frameAdapter.hpp"

DEFINE_CUSTOM_FORMATTING(leia::head::MovingPoint,
    "{{.pos={}, .vel={}}}", v.pos, v.vel);
DEFINE_CUSTOM_FORMATTING(leia::head::DetectedFace::Eye,
    "{{.imageCoord={}, .depth={}}}", v.imageCoord, v.depth);
DEFINE_CUSTOM_FORMATTING(leia::head::DetectedFace,
    "{{.eyes={{{}, {}}}, .posePosition={}, .poseAngle={}}}",
    v.eyes[leia::head::kRightEyeIdx], v.eyes[leia::head::kLeftEyeIdx], v.posePosition, v.poseAngle);
DEFINE_CUSTOM_FORMATTING(leia_headtracking_frame_profiling,
    "{{.cameraExposureTime={}, .faceDetectorStartTime={}, .faceDetectorEndTime={}, .apiTimestamp={}}}",
    v.cameraExposureTime, v.faceDetectorStartTime, v.faceDetectorEndTime, v.apiTimestamp);
