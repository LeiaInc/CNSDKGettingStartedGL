#ifndef CNSDK_LEIA_CORE_FACE_TRACKING_H
#define CNSDK_LEIA_CORE_FACE_TRACKING_H

#include "leia/core/api.h"
#include "leia/core/fwd.h"
#include "leia/headTracking/common/types.h"

BEGIN_CAPI_DECL

/*!
 * @defgroup faceTracking Face Tracking
 * @ingroup core
 *
 * @brief Face tracking on Leia devices.
 */

/*!
 * @addtogroup faceTracking
 * @{
 */

#ifdef LEIA_DOXYGEN
/**
 * @brief Face tracking frame listener.
 */
struct leia_headtracking_frame_listener {};
/**
 * @brief Face tracking frame.
 *
 * It contains all the data used by the face tracking engine.
 */
struct leia_headtracking_frame {};
#else
struct leia_headtracking_frame;
struct leia_headtracking_frame_listener;
#endif

/**
 * @brief Face Tracking frame callback.
 *
 * This callback is invoked from a background thread.
 * And it might still be called after disabling the listener due to asynchronicity.
 * Calling it must be valid until leia_free_user_data_cb has been called.
 *
 * Note: the handler is responsible for releasing the frame (@see leia_headtracking_frame_release)
 */
typedef void (*leia_headtracking_on_frame_cb)(OWNING(struct leia_headtracking_frame*) frame, void* userData);

/**
 * @brief Allocate face tracking frame listener.
 *
 * @param onFrameCb      frame callback
 * @param userData       user-provided pointer passed into callbacks [optional]
 * @param freeUserDataCb callback used to free the userData [optional]
 *
 * @relates leia_headtracking_frame_listener
 */
LEIA_NODISCARD
LEIASDK_API
OWNING(struct leia_headtracking_frame_listener*)
leia_headtracking_frame_listener_alloc(struct leia_core_library*,
                                       leia_headtracking_on_frame_cb onFrameCb,
                                       void*                         userData,
                                       leia_free_user_data_cb        freeUserDataCb);

/**
 * @brief Free face tracking frame listener.
 *
 * @public @memberof leia_headtracking_frame_listener
 */
LEIASDK_API
void leia_headtracking_frame_listener_release(OWNING(struct leia_headtracking_frame_listener*) listener);

/**
 * @public @memberof leia_headtracking_frame
 */
LEIASDK_API
leia_headtracking_status leia_headtracking_frame_get_tracking_result(struct leia_headtracking_frame*, struct leia_headtracking_tracking_result*);

/**
 * @public @memberof leia_headtracking_frame
 */
LEIASDK_API
leia_headtracking_status leia_headtracking_frame_get_raw_faces(struct leia_headtracking_frame*, struct leia_headtracking_raw_faces*);

/**
 * @public @memberof leia_headtracking_frame
 */
LEIASDK_API
leia_headtracking_status leia_headtracking_frame_get_detected_faces(struct leia_headtracking_frame*, struct leia_headtracking_detected_faces*);

/**
 * @public @memberof leia_headtracking_frame
 */
LEIASDK_API
leia_headtracking_status leia_headtracking_frame_get_profiling(struct leia_headtracking_frame*, struct leia_headtracking_frame_profiling*);

/**
 * @brief Get current camera-detected lux value if available.
 *
 * @public @memberof leia_headtracking_frame
 */
LEIASDK_API
float leia_headtracking_frame_get_lux(struct leia_headtracking_frame*);

/**
 * @public @memberof leia_headtracking_frame
 */
LEIASDK_API
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

/*!
 * @}
 */

END_CAPI_DECL

#endif // CNSDK_LEIA_CORE_FACE_TRACKING_H
