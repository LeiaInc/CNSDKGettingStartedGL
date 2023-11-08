#ifndef CNSDK_LEIA_CORE_CXX_ASSET_MANAGER_HPP
#define CNSDK_LEIA_CORE_CXX_ASSET_MANAGER_HPP

#include "leia/core/assetManager.h"

#include <string>

namespace leia {

class AssetManagerBuffer {
public:
    AssetManagerBuffer(OWNING(leia_asset_manager_buffer) buffer) : _buffer(buffer)
    {
    }
    ~AssetManagerBuffer()
    {
        leia_asset_manager_buffer_release(&_buffer);
    }
    LEIA_NODISCARD
    size_t GetSize() const
    {
        return _buffer.size;
    }
    LEIA_NODISCARD
    char* GetData() const
    {
        return _buffer.data;
    }

private:
    leia_asset_manager_buffer _buffer;
};

class AssetManager {
public:
    using ResolveContext = leia_asset_manager_resolve_context;
    AssetManager(NONOWNING(leia_asset_manager) * ptr) : _ptr(ptr)
    {
    }
    LEIA_NODISCARD
    AssetManagerBuffer ReadData(const char* assetPath, ResolveContext* resolveContext = nullptr) const
    {
        return {leia_asset_manager_read_data(_ptr, assetPath, resolveContext)};
    }
    LEIA_NODISCARD
    std::string ReadString(const char* assetPath, ResolveContext* resolveContext = nullptr) const
    {
        AssetManagerBuffer data = ReadData(assetPath, resolveContext);

        std::string ret;
        if (data.GetData() && data.GetSize())
        {
            ret = {data.GetData(), data.GetSize()};
        }
        return ret;
    }
    LEIA_NODISCARD
    leia_asset_manager* GetHandle()
    {
        return _ptr;
    }

private:
    leia_asset_manager* _ptr;
};

} // namespace leia

#endif // CNSDK_LEIA_CORE_CXX_ASSET_MANAGER_HPP
