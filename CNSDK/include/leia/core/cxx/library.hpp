#ifndef CNSDK_LEIA_CORE_CXX_LIBRARY_HPP
#define CNSDK_LEIA_CORE_CXX_LIBRARY_HPP

#include "leia/core/library.h"
#include "leia/core/cxx/exception.hpp"

namespace leia {

class CoreLibrary {
public:
    CoreLibrary() : _ptr(nullptr)
    {
    }
    CoreLibrary(leia_core_library_load_android const* android)
    {
        Load(android);
    }

    void Load(leia_core_library_load_android const* android)
    {
        leia_core_library_load_request loadRequest = {};

        loadRequest.apiVersion    = CNSDK_VERSION_U64;
        loadRequest.loaderVersion = LEIA_CORE_LOADER_API_VERSION;
        loadRequest.android       = android;
        _ptr                      = leia_core_library_load(&loadRequest);

#if !defined(LEIA_WRAPPER_DISABLE_EXCEPTION)
        if (!_ptr)
        {
            throw CoreInitFailed();
        }
#endif // !LEIA_WRAPPER_DISABLE_EXCEPTION
    }

    ~CoreLibrary()
    {
        if (_ptr)
        {
            leia_core_library_release(_ptr);
        }
    }

#if defined(LEIA_WRAPPER_DISABLE_EXCEPTION)
    LEIA_NODISCARD
    bool IsValid() const
    {
        return _ptr != nullptr;
    }
#endif // !LEIA_WRAPPER_DISABLE_EXCEPTION

    LEIA_NODISCARD
    leia_core_library* GetHandle() const
    {
        return _ptr;
    }

private:
    leia_core_library* _ptr;
};

} // namespace leia

#endif // CNSDK_LEIA_CORE_CXX_LIBRARY_HPP
