#ifndef CNSDK_LEIA_CORE_EVENT_CENTER_H
#define CNSDK_LEIA_CORE_EVENT_CENTER_H

#include "leia/core/api.h"
#include "leia/common/eventCenter.types.h"

BEGIN_CAPI_DECL

LEIA_NODISCARD
LEIASDK_API
struct leia_event_listener* leia_event_listener_alloc(struct leia_core_library*);
/// callback must be thread-safe.
LEIASDK_API
void leia_event_listener_set_callback(struct leia_event_listener*, leia_event_listener_callback callback, void* userData);
LEIASDK_API
void leia_event_listener_free(struct leia_event_listener*);

#if defined(LEIA_DOXYGEN)
/**
 * @brief Event center.
 *
 * Event center allows to listen for CNSDK events.
 */
struct leia_event_center {};
#endif // LEIA_DOXYGEN

/**
 * @brief Add event listener
 *
 * @note The listener should stay valid until it's not removed.
 *
 * @public @memberof leia_event_center
 */
LEIASDK_API
void leia_event_center_add_listener(NONOWNING(struct leia_event_listener*) listener);

/**
 * @brief Remove event listener
 *
 * @public @memberof leia_event_center
 */
LEIASDK_API
void leia_event_center_remove_listener(NONOWNING(struct leia_event_listener*) listener);

END_CAPI_DECL

#endif // CNSDK_LEIA_CORE_EVENT_CENTER_H
