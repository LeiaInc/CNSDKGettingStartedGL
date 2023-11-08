#ifndef CNSDK_LEIA_CORE_INTERLACER_VULKAN_H
#define CNSDK_LEIA_CORE_INTERLACER_VULKAN_H

#include "leia/core/interlacer.h"

// TODO: Why don't we just include the header?
// Note: We forward-declare Vulkan objects to avoid including the Vulkan SDK header.
//       Unfortunately we need two untyped enums VkFormat and VkImageLayout that can't be forward-declared on some compilers.
//       Our solution is to declare and use two integer types VkFormatInt and VkImageLayoutInt that can safely be cast to/from the Vulkan enums.
typedef struct VkDevice_T*         VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkFramebuffer_T*    VkFramebuffer;
typedef struct VkImage_T*          VkImage;
typedef struct VkImageView_T*      VkImageView;
typedef struct VkSemaphore_T*      VkSemaphore;
typedef struct VkCommandBuffer_T*  VkCommandBuffer;
typedef struct VkRenderPass_T*     VkRenderPass;
typedef struct VkQueue_T*          VkQueue;
typedef int32_t                    VkFormatInt;
typedef int32_t                    VkImageLayoutInt;

#if defined(LEIA_USE_VULKAN)

BEGIN_CAPI_DECL

/*!
 * @defgroup interlacer_vulkan Vulkan Interlacer
 * @ingroup interlacer
 *
 * @brief Vulkan interlacing on Leia devices.
 */

/*!
 * @addtogroup interlacer_vulkan
 * @{
 */

/**
 * @brief Initialize interlacer for usage with Vulkan.
 *
 * If your application uses Vulkan, use this method to initialize.
 *
 * @param[in] device Vulkan device.
 * @param[in] physicalDevice Vulkan physical device.
 * @param[in] renderTargetFormat Pixel format to use for render-targets.
 * @param[in] depthStencilFormat Pixel format to use for depth-stencil targets.
 * @param[in] maxInFlightFrameCount Maximum number of frames rendering simultaneously.
 *
 * @relates leia_interlacer
 */
LEIA_NODISCARD
LEIASDK_API
struct leia_interlacer* leia_interlacer_vulkan_initialize(struct leia_core*,
                                                          struct leia_interlacer_init_configuration const*,
                                                          VkDevice         device,
                                                          VkPhysicalDevice physicalDevice,
                                                          VkFormatInt      renderTargetFormat,
                                                          VkFormatInt      depthStencilFormat,
                                                          int32_t          maxInFlightFrameCount);

/**
 * @brief Perform Vulkan graphics rendering.
 *
 * Use this method to perform all graphics rendering - interlacing, sharpening, and GUI display.
 *
 * @param[in] width Width of the render-target.
 * @param[in] height Height of the render-target.
 * @param[in] blendedInterlace Whether to blend the graphics output onto the render-target.
 * @param[in] frameBuffer Destination frame buffer for interlaced output.
 * @param[in] colorImage Destination frame buffer color image.
 * @param[in] depthImage Destination frame buffer depth image.
 * @param[in] imageAvailableSemaphore Semaphore to wait on before rendering.
 * @param[in] renderFinishedSemaphore Semaphore to signal once rendering is complete.
 * @param[in] currentFrame The current frame index.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_vulkan_do_post_process(struct leia_interlacer*,
                                            int32_t       width,
                                            int32_t       height,
                                            leia_bool     blendedInterlace,
                                            VkFramebuffer frameBuffer,
                                            VkImage       colorImage,
                                            VkImage       depthImage,
                                            VkSemaphore   imageAvailableSemaphore,
                                            VkSemaphore   renderFinishedSemaphore,
                                            int32_t       currentFrame);

/**
 * @brief Perform Vulkan graphics rendering for picture content.
 *
 * Use this method to perform all graphics rendering - interlacing, sharpening, and GUI display. This method
 * is similar to DoPostProcess() but allows you to pass in the views to interlace and the destination render-target.
 *
 * @param[in] width Width of the render-target.
 * @param[in] height Height of the render-target.
 * @param[in] pictureResource Views texture atlas.
 * @param[in] frameBuffer Destination frame buffer for interlaced output.
 * @param[in] colorImage Destination frame buffer color image.
 * @param[in] depthImage Destination frame buffer depth image.
 * @param[in] imageAvailableSemaphore Semaphore to wait on before rendering.
 * @param[in] renderFinishedSemaphore Semaphore to signal once rendering is complete.
 * @param[in] currentFrame The current frame index.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_vulkan_do_post_process_picture(struct leia_interlacer*,
                                                    int32_t       width,
                                                    int32_t       height,
                                                    VkImage       pictureResource,
                                                    VkFramebuffer frameBuffer,
                                                    VkImage       colorImage,
                                                    VkImage       depthImage,
                                                    VkSemaphore   imageAvailableSemaphore,
                                                    VkSemaphore   renderFinishedSemaphore,
                                                    int32_t       currentFrame);

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
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_vulkan_set_source_views(struct leia_interlacer*, VkImage texture, VkImageView textureView, int32_t viewIndex, int layer);

/*!
 * @}
 */

END_CAPI_DECL

#endif // LEIA_USE_VULKAN

#endif // CNSDK_LEIA_CORE_INTERLACER_VULKAN_H
