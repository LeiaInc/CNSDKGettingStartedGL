#ifndef CNSDK_LEIA_DEVICE_CONFIG_H
#define CNSDK_LEIA_DEVICE_CONFIG_H

#include "leia/common/types.h"

#pragma pack(push, 8)

struct leia_device_config {
    float     dotPitchInMM[2];
    int32_t   panelResolution[2];
    int32_t   numViews[2];
    int32_t   sharpeningKernelXSize;
    float     sharpeningKernelX[18];
    int32_t   sharpeningKernelYSize;
    float     sharpeningKernelY[18];
    int32_t   viewResolution[2];
    int32_t   displaySizeInMm[2];
    float     act_gamma;
    float     act_beta;
    float     act_singleTapCoef;
    float     systemDisparityPercent;
    float     systemDisparityPixels;
    float     cameraCenterX;
    float     cameraCenterY;
    float     cameraCenterZ;
    float     cameraThetaX;
    float     cameraThetaY;
    float     cameraThetaZ;
    float     centerViewNumber;
    float     convergence;
    float     n;
    float     theta;
    float     s;
    float     d_over_n;
    float     p_over_du;
    float     p_over_dv;
    leia_bool colorInversion;
    int32_t   colorSlant;
    int32_t   cameraWidth;
    int32_t   cameraHeight;
    int32_t   cameraFps;
    float     cameraBinningFactor;
    float     facePredictAlphaX;
    float     facePredictAlphaY;
    float     facePredictAlphaZ;
    float     facePredictBeta;
    float     facePredictLatencyMs;
    float     accelerationThreshold;

    enum leia_orientation deviceNaturalOrientation;
    enum leia_orientation cameraSensorOrientation;

    int32_t overlay;
    int32_t isOverlayDeviceSwitchable;
    float   smooth;
    float   phc;
    float   p1;
    float   p2;
    float   sl1;
    float   sl2;
    float   subpixCentersX[3];
    float   subpixCentersY[3];

    leia_bool faceTrackingSingleFaceEnable;
    float     faceTrackingSingleFaceTooFarDistanceThreshold;
    int32_t   faceTrackingSingleFaceTooFarResetTimeoutMs;
    int32_t   faceTrackingMaxNumOfFaces;

    float faceTrackingHeadPoseZLowPassAlpha;

    char rShiftX[128];
    char rShiftY[128];
    char gShiftX[128];
    char gShiftY[128];
    char bShiftX[128];
    char bShiftY[128];

    struct leia_normalized_camera_intrinsics trackingCameraIntrinsics;
};

#pragma pack(pop)

#endif // CNSDK_LEIA_DEVICE_CONFIG_H
