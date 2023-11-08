#ifndef CNSDK_LEIA_CORE_CXX_EVENT_CENTER_HPP
#define CNSDK_LEIA_CORE_CXX_EVENT_CENTER_HPP

#include "leia/core/eventCenter.h"

namespace leia {

using Event                 = leia_event;
using EventListenerCallback = leia_event_listener_callback;

class EventListener {
public:
    EventListener(CoreLibrary const& coreLib, EventListenerCallback callback, void* userData) : _listener(leia_event_listener_alloc(coreLib.GetHandle()))
    {
        leia_event_listener_set_callback(_listener, callback, userData);
    }
    ~EventListener()
    {
        Unregister();
        leia_event_listener_free(_listener);
    }
    void Register()
    {
        leia_event_center_add_listener(_listener);
    }
    void Unregister()
    {
        leia_event_center_remove_listener(_listener);
    }

private:
    leia_event_listener* _listener;
};

} // namespace leia

#endif // CNSDK_LEIA_CORE_CXX_EVENT_CENTER_HPP
