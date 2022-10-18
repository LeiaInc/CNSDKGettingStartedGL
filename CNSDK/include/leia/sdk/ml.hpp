#pragma once

#include "leia/sdk/api.h"

#if defined(LEIA_OS_WINDOWS)
#include "leia/common/windows/windowsPlatform.hpp"
#elif defined(LEIA_OS_ANDROID)
#include "leia/common/android/androidPlatform.hpp"
#endif

#include <functional>
#include <memory>

struct leia_sdk_image_desc;

namespace leia { class Platform; class AssetManager; }

namespace leia::sdk {

class MLImage;
struct LeiaMediaSdkPlugin;
struct MLMultiviewConfiguration;

class ML {
public:
    virtual ~ML();

    using TiledImageCallback = std::function<void(std::unique_ptr<MLImage> albedo, std::unique_ptr<MLImage> disparity)>;

    virtual bool ConvertSync(fs::path const& singleImagePath, MLMultiviewConfiguration const&, TiledImageCallback const&) = 0;
    virtual void ConvertAsync(fs::path const& singleImagePath, MLMultiviewConfiguration const&, TiledImageCallback const&) = 0;

    virtual bool ConvertSync(leia_sdk_image_desc const& singleImage, MLMultiviewConfiguration const&, TiledImageCallback const&) = 0;
    virtual void ConvertAsync(std::shared_ptr<MLImage> const& singleImage, MLMultiviewConfiguration const&, TiledImageCallback const&) = 0;

#if defined(LEIA_OS_ANDROID)
    virtual jobject ConvertSync(JNIEnv*, jobject albedoBitmap, int numViews) = 0;
#endif
};

class MLImage {
public:
    LEIASDK_API
    virtual ~MLImage() = 0;
    virtual leia_sdk_image_desc GetDesc() const = 0;
};

struct MLMultiviewConfiguration {
    int width = -1;
    int height = -1;
    float gain = -9999;
    float gainMultiplier = -9999;
    float convergence = -9999;
    int numViews;

    MLMultiviewConfiguration(int numViews) : numViews(numViews) {}
};

std::unique_ptr<ML> CreateML(Platform*, AssetManager*);

} // namespace leia::sdk
