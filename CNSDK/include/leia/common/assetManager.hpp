#pragma once

#include "leia/common/api.h"

#if defined(LEIA_OS_ANDROID)
# include "leia/common/android/androidPlatform.hpp"
#elif defined(LEIA_OS_WINDOWS)
# include "leia/common/windows/windowsPlatform.hpp"
#endif

namespace leia {

class AssetManager {
public:
    LEIA_COMMON_API
    static std::unique_ptr<AssetManager> Create(Platform*);
    LEIA_COMMON_API
    virtual ~AssetManager();

    struct ResolveContext {
#if defined(LEIA_OS_WINDOWS)
        /// If specified, used as a base directory for relative assetPaths.
        fs::path baseDir;
        /// Allows automatic base dir evaluation based on an address from a module (.dll, .exe).
        /// See windowsPlatform's GetLibraryPath(void*).
        void* moduleAddress = nullptr;
#endif
    };

    // TODO: add function that returns read buffer (std::fstream::rdbuf) for efficient .tga parsing

    struct Buffer {
        size_t size;
        std::unique_ptr<char[]> data;
    };
    virtual Buffer ReadData(std::string const& assetPath, ResolveContext* resolveContext = nullptr) = 0;

    virtual std::string ReadString(std::string const& assetPath, ResolveContext* resolveContext = nullptr) = 0;

    virtual fs::path UnpackDir(std::string const& dirName, ResolveContext* resolveContext = nullptr) = 0;

protected:
    AssetManager();
};

#if defined(LEIA_OS_ANDROID)
fs::path const& GetCacheDir(AssetManager*);
#endif

} // namespace leia
