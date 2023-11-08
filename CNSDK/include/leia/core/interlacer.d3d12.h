#ifndef CNSDK_LEIA_CORE_INTERLACER_D3D12_H
#define CNSDK_LEIA_CORE_INTERLACER_D3D12_H

#include "leia/core/interlacer.h"

#if defined(LEIA_USE_DIRECTX12)

struct ID3D12Device;
struct ID3D12CommandQueue;
struct ID3D12Resource;
struct CD3DX12_CPU_DESCRIPTOR_HANDLE;
struct D3D12_CPU_DESCRIPTOR_HANDLE;
typedef enum D3D12_RESOURCE_STATES;

BEGIN_CAPI_DECL

/*!
 * @defgroup interlacer_d3d12 D3D12 Interlacer
 * @ingroup interlacer
 *
 * @brief D3D12 interlacing on Leia devices.
 */

/*!
 * @addtogroup interlacer_d3d12
 * @{
 */

/**
 * @brief Initialize interlacer for usage with Direct3D 12.
 *
 * If your application uses Direct3D 12, use this method to initialize.
 *
 * @param[in] device Direct3D 12 device.
 * @param[in] commandQueue Command queue to use.
 *
 * @relates leia_interlacer
 */
LEIA_NODISCARD
LEIASDK_API
struct leia_interlacer* leia_interlacer_d3d12_initialize(struct leia_core*                                core,
                                                         struct leia_interlacer_init_configuration const* config,
                                                         struct ID3D12Device*                             device,
                                                         struct ID3D12CommandQueue*                       commandQueue);

/**
 * @brief Perform Direct3D 11 graphics rendering.
 *
 * Use this method to perform all graphics rendering - interlacing, sharpening, and GUI display.
 *
 * @param[in] width Width of the render-target.
 * @param[in] height Height of the render-target.
 * @param[in] blendedInterlace Whether to blend the graphics output onto the render-target.
 * @param[in] renderTarget Destination render-target for interlaced output.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_d3d12_do_post_process(
    struct leia_interlacer*, int32_t width, int32_t height, leia_bool blendedInterlace, struct ID3D12Resource* renderTarget);

/**
 * @brief Perform Direct3D 12 graphics rendering for picture content.
 *
 * Use this method to perform all graphics rendering - interlacing, sharpening, and GUI display. This method
 * is similar to DoPostProcess() but allows you to pass in the views to interlace and the destination render-target.
 *
 * @param[in] width Width of the render-target.
 * @param[in] height Height of the render-target.
 * @param[in] pictureResource Views texture atlas.
 * @param[in] renderTarget Destination render-target for interlaced output.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_d3d12_do_post_process_picture(
    struct leia_interlacer*, int32_t width, int32_t height, struct ID3D12Resource* pictureResource, struct ID3D12Resource* renderTarget);

/**
 * @brief Set Direct3D 12 view texture.
 *
 * This method allows you to specify the texture for a specific view index and layer.
 *
 * @param[in] resource The view texture.
 * @param[in] viewIndex The view index.
 * @param[in] layer The layer at which to set the view texture.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_d3d12_set_source_views(struct leia_interlacer*, struct ID3D12Resource* resource, int viewIndex, int layer);

/*!
 * @}
 */

END_CAPI_DECL

#endif // LEIA_USE_DIRECTX12

#endif // CNSDK_LEIA_CORE_INTERLACER_D3D12_H
