#pragma once

#include "leia/headTracking/service/api.h"
#include "leia/headTracking/service/common.hpp"
#include "leia/common/platform.hpp"

#include <functional>
#include <memory>

namespace leia::head { class FrameAdapter; }

namespace leia::head::service {

struct ClientListener {
    virtual void OnConnect(ServerInfo const&) = 0;
    virtual void OnFaceDetectorBackendUpdate(FaceDetectorBackend) = 0;
    virtual void OnDisconnect() = 0;
};

struct ClientInitArgs {
    FrameAdapter* frameAdapter = nullptr;
    Platform* platform = nullptr;

    ClientListener* listener = nullptr;

    ServerInitArgs server;
};

class Client {
public:
    LHT_SERVICE_API
    Client(ClientInitArgs const&);
    LHT_SERVICE_API
    ~Client();

    LHT_SERVICE_API
    void StartTracking();
    LHT_SERVICE_API
    void StopTracking();

    LHT_SERVICE_API
    void SetFaceDetectorBackend(FaceDetectorBackend);
    LHT_SERVICE_API
    void SetServerLogLevel(leia_log_level);
    LHT_SERVICE_API
    void SetProfiling(bool enable);

private:
    void InitServerConnection(ClientInitArgs const&);

private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace leia::head::service
