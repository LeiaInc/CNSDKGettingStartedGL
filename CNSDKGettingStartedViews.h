#pragma once

// Helper function to compute the following per-view values:
//    - Position
//    - Projection matrix
//    - Field-of-view
//    - Shear
//    - Comfort zone.
void GetViewInfo
(
    int          viewIndex,                         // [In] Index of view. For example 0 (left) or 1 (right) for stereo.
    float        sdkBaselineScaling,                // [In] Baseline scaling value from Leia SDK
    const vec3f& interlacerViewOffset,              // [In] View offset from Leia Interlacer
    const vec3f& cameraPos,                         // [In] Camera position.
    const vec3f& cameraDir,                         // [In] Camera forward direction (unit vector).
    const vec3f& cameraUp,                          // [In] Camera up direction (unit vector).
    float        perspectiveFieldOfView,            // [In] The field of view for perspective projections. If zero, perspectiveFieldOfView is auto-computed based on (required) convergence plane distance and (optional) convergence plane height. Must be zero for orthographc projections.
    float        perspectiveAspectRatio,            // [In] Aspect ratio for perspective projections. Must be zero for orthographc projections.
    float        orthoWidth,                        // [In] Width of ortographic projection. Must be zero for perspective projection.
    float        orthoHeight,                       // [In] Height of ortographic projection. Must be zero for perspective projection.
    float        nearPlane,                         // [In] Frustum near plane.
    float        farPlane,                          // [In] Frustum far plane.
    float        convergencePlaneDistance,          // [In] Convergence plane distance. If zero, will default to middle of frustum near/far planes. Ignored for perspective projections with non-zero perspectiveFieldOfView.
    float        convergencePlaneHeight,            // [In] Convergence plane height. If zero, will match the distance. Ignored for perspective projections with non-zero perspectiveFieldOfView.
    vec3f*       viewPos              = nullptr,    // [Out] Position of camera for specified view.
    mat4f*       viewProjectionMatrix = nullptr,    // [Out] Projection matrix for specified view.
    float*       viewFieldOfView      = nullptr,    // [Out] Field-of-view for specified view (only for perspective projections where FOV is auto-computed, i.e. perspectiveFieldOfView is zero).
    float*       viewShearX           = nullptr,    // [Out] X shear factor.
    float*       viewShearY           = nullptr,    // [Out] X shear factor.
    float*       viewComfortZoneNear  = nullptr,    // [Out] Near comfort zone. For best 3d results, keep geometry beyond this distance.
    float*       viewComfortZoneFar   = nullptr     // [Out] Far comfort zone. For best 3d results, keep geometry before this distance.
)
{
    // Make sure inputs are either orthographic or perspective, but not both.
    bool isOrthographic = (orthoWidth > 0) && (orthoHeight > 0) && (perspectiveAspectRatio == 0);
    bool isPerspective = (orthoWidth == 0) && (orthoHeight == 0) && (perspectiveAspectRatio > 0);
    assert(0 != (isOrthographic ^ isPerspective));

    // If the convergence plane distance isn't set, use middle of the frustum.
    if (convergencePlaneDistance <= 0.0f)
        convergencePlaneDistance = (nearPlane + farPlane) / 2.0f;

    // If the convergence plane height isn't set, use same value as distance.
    if (convergencePlaneHeight <= 0.0f)
        convergencePlaneHeight = convergencePlaneDistance;

    assert((nearPlane <= convergencePlaneDistance) && (convergencePlaneDistance <= farPlane));

    // Compute camera right vector.
    const vec3f cameraRight = vec3f::cross(cameraDir, cameraUp);

    // Compute view camera position.
    const vec3f camPos = cameraPos + (cameraRight * interlacerViewOffset.x) + (cameraDir * interlacerViewOffset.z) + (cameraUp * interlacerViewOffset.y);
    if (viewPos != nullptr)
        *viewPos = camPos;

    // Compute shear.
    const float shearX = -interlacerViewOffset.x / convergencePlaneDistance;
    const float shearY = -interlacerViewOffset.z / convergencePlaneDistance;
    if (viewShearX != nullptr)
        *viewShearX = shearX;
    if (viewShearY != nullptr)
        *viewShearY = shearX;

    // Compute projection matrix and comfort zone range.
    mat4f projectionMatrix;
    float comfortZoneNear = 0.0f;
    float comfortZoneFar = 0.0f;
    if (isPerspective)
    {
        assert((perspectiveFieldOfView > 0.0f) || (convergencePlaneDistance > 0.0f));
        assert(perspectiveAspectRatio > 0.0f);
        assert((nearPlane > 0.0f) && (nearPlane < farPlane));
        assert(perspectiveAspectRatio > 0.0f);

        // Use provided FOV or compute FOV to match desired plane height at specified distance.
        const float fov = (perspectiveFieldOfView > 0.0f) ? perspectiveFieldOfView : (2.0f * atan(convergencePlaneHeight / (2.0f * convergencePlaneDistance)));
        if (viewFieldOfView != nullptr)
            *viewFieldOfView = fov;

        // Create projection matrix.
        projectionMatrix.setPerspective(fov, perspectiveAspectRatio, nearPlane, farPlane);
        
        // Compute comfort zone.
        comfortZoneNear = (sdkBaselineScaling * convergencePlaneDistance) / (sdkBaselineScaling + 1.0f);
        comfortZoneFar = (sdkBaselineScaling * convergencePlaneDistance) / (sdkBaselineScaling - 1.0f);
    }
    else if (isOrthographic)
    {
        // Create projection matrix.
        projectionMatrix.setOrthographic(-orthoWidth / 2.0f, orthoWidth / 2.0f, -orthoHeight / 2.0f, orthoHeight / 2.0f, nearPlane, farPlane);

        // Compute comfort zone.
        comfortZoneNear = convergencePlaneDistance - (1.0f / sdkBaselineScaling);
        comfortZoneFar  = convergencePlaneDistance + (1.0f / sdkBaselineScaling);
    }
    else
    {
        assert(false);
    }

    // Apply shear to projection matrix.
    projectionMatrix[2][0] = projectionMatrix[0][0] * shearX;
    projectionMatrix[2][1] = projectionMatrix[1][1] * shearY;
    if (viewProjectionMatrix != nullptr)
        *viewProjectionMatrix = projectionMatrix;

    // Return comfort zone.
    if (viewComfortZoneNear != nullptr)
        *viewComfortZoneNear = comfortZoneNear;
    if (viewComfortZoneFar != nullptr)
        *viewComfortZoneFar = comfortZoneFar;
}

// Get view with standard orthographic projection and convergence plane in middle of frustum.
void GetStandardOrthographicViewInfo
(
    int viewIndex,
    float sdkBaselineScaling,
    const vec3f& interlacerViewOffset,
    const vec3f& cameraPos,
    const vec3f& cameraDir,
    const vec3f& cameraUp,
    float width,
    float height,
    float nearPlane,
    float farPlane,
    vec3f* viewPos = nullptr,
    mat4f* viewProjectionMatrix = nullptr,
    float* viewShearX = nullptr,
    float* viewShearY = nullptr
)
{
    return GetViewInfo
    (
        viewIndex,
        sdkBaselineScaling,
        interlacerViewOffset,
        cameraPos,
        cameraDir,
        cameraUp,
        0.0f,
        0.0f,
        width,
        height,
        nearPlane,
        farPlane,
        0.0f,
        0.0f,
        viewPos,
        viewProjectionMatrix,
        viewShearX,
        viewShearY
    );
}

// Get view with orthographic projection for convergence plane at specified distance.
void GetConvergedOrthographicViewInfo
(
    int viewIndex,
    float sdkBaselineScaling,
    const vec3f& interlacerViewOffset,
    const vec3f& cameraPos,
    const vec3f& cameraDir,
    const vec3f& cameraUp,
    float width,
    float height,
    float nearPlane,
    float farPlane,
    float convergencePlaneDistance,
    float convergencePlaneHeight,
    vec3f* viewPos = nullptr,
    mat4f* viewProjectionMatrix = nullptr,
    float* viewFieldOfView = nullptr,
    float* viewShearX = nullptr,
    float* viewShearY = nullptr
)
{
    return GetViewInfo
    (
        viewIndex,
        sdkBaselineScaling,
        interlacerViewOffset,
        cameraPos,
        cameraDir,
        cameraUp,
        0.0f,
        0.0f,
        width,
        height,
        nearPlane,
        farPlane,
        convergencePlaneDistance,
        convergencePlaneHeight,
        viewPos,
        viewProjectionMatrix,
        viewShearX,
        viewShearY
    );
}

// Get view with standard perspective projection and convergence plane in middle of frustum.
void GetStandardPerspectiveViewInfo
(
    int viewIndex,
    float sdkBaselineScaling,
    const vec3f& interlacerViewOffset,
    const vec3f& cameraPos,
    const vec3f& cameraDir,
    const vec3f& cameraUp,
    float fov,
    float aspectRatio,
    float nearPlane,
    float farPlane,
    vec3f* viewPos = nullptr,
    mat4f* viewProjectionMatrix = nullptr,
    float* viewShearX = nullptr,
    float* viewShearY = nullptr
)
{
    return GetViewInfo
    (
        viewIndex,
        sdkBaselineScaling,
        interlacerViewOffset,
        cameraPos,
        cameraDir,
        cameraUp,
        fov,
        aspectRatio,
        0.0f,
        0.0f,
        nearPlane,
        farPlane,
        0.0f,
        0.0f,
        viewPos,
        viewProjectionMatrix,
        viewShearX,
        viewShearY
    );
}

// Get view with perspective projection for convergence plane at specified distance.
void GetConvergedPerspectiveViewInfo
(
    int viewIndex,
    float sdkBaselineScaling,
    const vec3f& interlacerViewOffset,
    const vec3f& cameraPos,
    const vec3f& cameraDir,
    const vec3f& cameraUp,
    float aspectRatio,
    float nearPlane,
    float farPlane,
    float convergencePlaneDistance,
    float convergencePlaneHeight,
    vec3f* viewPos = nullptr,
    mat4f* viewProjectionMatrix = nullptr,
    float* viewFieldOfView = nullptr,
    float* viewShearX = nullptr,
    float* viewShearY = nullptr
)
{
    return GetViewInfo
    (
        viewIndex,
        sdkBaselineScaling,
        interlacerViewOffset,
        cameraPos,
        cameraDir,
        cameraUp,
        0.0f,
        aspectRatio,
        0.0f,
        0.0f,
        nearPlane,
        farPlane,
        convergencePlaneDistance,
        convergencePlaneHeight,
        viewPos,
        viewProjectionMatrix,
        viewShearX,
        viewShearY
    );
}