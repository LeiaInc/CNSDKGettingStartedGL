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
#ifndef CNSDK_LEIA_SDK_CORE_INTERLACER_VULKAN_H
#define CNSDK_LEIA_SDK_CORE_INTERLACER_VULKAN_H

#include "leia/sdk/core.interlacer.h"

#if !defined(LEIA_USE_VULKAN)
#    error "CNSDK is not compiled with Vulkan support"
#endif

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
 * @param[in] textureFormat Pixel format to use for textures.
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
                                                          VkFormatInt      textureFormat,
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
 * @brief Set Vulkan view texture atlas.
 *
 * If your application is managing views, this method allows you to specify your texture atlas
 * containing all views.
 *
 * @param[in] texture The image containing all views.
 * @param[in] textureView The image view object.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_vulkan_set_interlace_view_texture_atlas(struct leia_interlacer*, VkImage texture, VkImageView textureView, int layer);

/**
 * @brief Set Vulkan individual view texture.
 *
 * If your application is managing views, this method allows you to specify the texture
 * for a specific view index.
 *
 * @param[in] viewIndex The view index.
 * @param[in] imageView The individual image view.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_vulkan_set_view_for_texture_array(struct leia_interlacer*, int32_t viewIndex, VkImageView imageView, int layer);

/**
 * @brief Set the Vulkan view texture for stereo picture or video content.
 *
 * If you are using DoPostProcessPicture() or DoPostProcessVideo(), you can use this method to specify
 * the input views texture rather than passing it to the PostProcess methods directly.
 *
 * @param[in] view Image for the stereo views texture.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_vulkan_set_view_texture_id(struct leia_interlacer*, VkImage view);

/*!
 * @}
 */

END_CAPI_DECL

#endif // CNSDK_LEIA_SDK_CORE_INTERLACER_VULKAN_H
