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
#ifndef CNSDK_LEIA_COMMON_SHARED_CAMERA_SINK_H
#define CNSDK_LEIA_COMMON_SHARED_CAMERA_SINK_H

#include "leia/common/api.h"
#include "leia/common/types.h"

BEGIN_CAPI_DECL

#if defined(LEIA_DOXYGEN)
struct leia_shared_camera_sink {};
#endif

/**
 * @brief Create a shared camera sink.
 *
 * The only goal of this object is to connect a camera data producer with a consumer.
 * Right now, the only existing consumer is the face tracking backend.
 *
 * This object is reference counted. See @ref md_reference_counting_rules.
 *
 * @relates leia_shared_camera_sink
 */
LEIA_NODISCARD
LEIA_COMMON_API
struct leia_shared_camera_sink* leia_shared_camera_sink_alloc(void);

/**
 * @brief Increment the reference counter.
 *
 * @public @memberof leia_shared_camera_sink
 */
LEIA_COMMON_API
void leia_shared_camera_sink_retain(struct leia_shared_camera_sink*);

/**
 * @brief Decrement the reference counter.
 *
 * @public @memberof leia_shared_camera_sink
 */
LEIA_COMMON_API
void leia_shared_camera_sink_release(struct leia_shared_camera_sink*);

/**
 * @deprecated Use leia_shared_camera_sink_release instead.
 *
 * @public @memberof leia_shared_camera_sink
 */
LEIA_DEPRECATED
LEIA_COMMON_API
void leia_shared_camera_sink_free(struct leia_shared_camera_sink*);

/**
 * @brief Check if shared camera sink is active (attached to a consumer).
 *
 * @public @memberof leia_shared_camera_sink
 */
LEIA_NODISCARD
LEIA_COMMON_API
leia_bool leia_shared_camera_sink_is_active(struct leia_shared_camera_sink*);

/**
 * @brief Push image data to the consumer.
 *
 * Makes sense to be called only when the sink is active.
 * See leia_shared_camera_sink_is_active.
 *
 * @public @memberof leia_shared_camera_sink
 */
LEIA_COMMON_API
void leia_shared_camera_sink_on_image(struct leia_shared_camera_sink*, struct leia_image_desc const*, struct leia_timestamp);

/**
 * @brief Push intrinsics data to the consumer.
 *
 * Makes sense to be called only when the sink is active.
 * See leia_shared_camera_sink_is_active.
 *
 * @public @memberof leia_shared_camera_sink
 */
LEIA_COMMON_API
void leia_shared_camera_sink_on_intrinsics_change(struct leia_shared_camera_sink*, struct leia_camera_intrinsics const*);

/**
 * @brief Push lux data to the consumer.
 *
 * Makes sense to be called only when the sink is active.
 * See leia_shared_camera_sink_is_active.
 *
 * @public @memberof leia_shared_camera_sink
 */
LEIA_COMMON_API
void leia_shared_camera_sink_on_lux_change(struct leia_shared_camera_sink*, float luxValue);

END_CAPI_DECL

#endif // CNSDK_LEIA_COMMON_SHARED_CAMERA_SINK_H
