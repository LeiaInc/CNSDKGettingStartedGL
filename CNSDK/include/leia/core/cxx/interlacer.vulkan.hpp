#ifndef CNSDK_LEIA_CORE_CXX_INTERLACER_VULKAN_HPP
#define CNSDK_LEIA_CORE_CXX_INTERLACER_VULKAN_HPP

#include "leia/core/cxx/interlacer.hpp"
#include "leia/core/interlacer.vulkan.h"

/*!
 * @addtogroup interlacer_vulkan
 * @{
 */

namespace leia {

/**
 * @brief C++ wrapper for Vulkan leia_interlacer
 */
class InterlacerVulkan : public Interlacer {
public:
    InterlacerVulkan(Core&                              core,
                     InterlacerInitConfiguration const& config,
                     VkDevice                           device,
                     VkPhysicalDevice                   physicalDevice,
                     VkFormatInt                        renderTargetFormat,
                     VkFormatInt                        depthStencilFormat,
                     int32_t                            maxInFlightFrameCount)
        : Interlacer(leia_interlacer_vulkan_initialize(
                         core.GetHandle(), config.GetHandle(), device, physicalDevice, renderTargetFormat, depthStencilFormat, maxInFlightFrameCount),
                     true)
    {
    }
    InterlacerVulkan(InterlacerVulkan const&) = delete;
    InterlacerVulkan& operator=(InterlacerVulkan const&) = delete;
    InterlacerVulkan(InterlacerVulkan&&) = delete;
    InterlacerVulkan& operator=(InterlacerVulkan&&) = delete;

    void DoPostProcess(int32_t       width,
                       int32_t       height,
                       bool          blendedInterlace,
                       VkFramebuffer frameBuffer,
                       VkImage       colorImage,
                       VkImage       depthImage,
                       VkSemaphore   imageAvailableSemaphore,
                       VkSemaphore   renderFinishedSemaphore,
                       int32_t       currentFrame)
    {
        leia_interlacer_vulkan_do_post_process(
            _interlacer, width, height, blendedInterlace, frameBuffer, colorImage, depthImage, imageAvailableSemaphore, renderFinishedSemaphore, currentFrame);
    }

    void DoPostProcessPicture(int32_t       width,
                              int32_t       height,
                              VkImage       pictureResource,
                              VkFramebuffer frameBuffer,
                              VkImage       colorImage,
                              VkImage       depthImage,
                              VkSemaphore   imageAvailableSemaphore,
                              VkSemaphore   renderFinishedSemaphore,
                              int32_t       currentFrame)
    {
        leia_interlacer_vulkan_do_post_process_picture(
            _interlacer, width, height, pictureResource, frameBuffer, colorImage, depthImage, imageAvailableSemaphore, renderFinishedSemaphore, currentFrame);
    }

    /**
     * @brief Set Vulkan view texture.
     *
     * This method allows you to specify the texture for a specific view index and layer.
     *
     * @param[in] texture The view texture.
     * @param[in] textureView The view texture's image-view.
     * @param[in] viewIndex The view index.
     * @param[in] layer The layer at which to set the view texture.
     *
     * @public @memberof InterlacerVulkan
     */
    void SetSourceViews(VkImage texture, VkImageView textureView, int32_t viewIndex = 0, int layer = 0)
    {
        leia_interlacer_vulkan_set_source_views(_interlacer, texture, textureView, viewIndex, layer);
    }

    InterlacerVulkan(leia_interlacer* nonOwningInterlacer) : Interlacer(nonOwningInterlacer, false)
    {
    }
};

LEIA_NODISCARD
inline InterlacerVulkan* AsVulkan(Interlacer* interlacer)
{
    if (interlacer && interlacer->GetGraphicsAPI() == LEIA_GRAPHICS_API_VULKAN)
    {
        return static_cast<InterlacerVulkan*>(interlacer);
    }
    return nullptr;
}

} // namespace leia

/*!
 * @}
 */

#endif // CNSDK_LEIA_CORE_CXX_INTERLACER_VULKAN_HPP