#ifndef CNSDK_LEIA_CORE_INTERLACER_D3D11_H
#define CNSDK_LEIA_CORE_INTERLACER_D3D11_H

#include "leia/core/interlacer.h"

#if defined(LEIA_USE_DIRECTX)

struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;

BEGIN_CAPI_DECL

/*!
 * @defgroup interlacer_d3d11 D3D11 Interlacer
 * @ingroup interlacer
 *
 * @brief D3D11 interlacing on Leia devices.
 */

/*!
 * @addtogroup interlacer_d3d11
 * @{
 */

/**
 * @brief Initialize interlacer for usage with Direct3D 11.
 *
 * If your application uses Direct3D 11, use this method to initialize.
 *
 * @param[in] context Pointer to your ID3D11DeviceContext object.
 *
 * @relates leia_interlacer
 */
LEIA_NODISCARD
LEIASDK_API
struct leia_interlacer* leia_interlacer_d3d11_initialize(struct leia_core*                                core,
                                                         struct leia_interlacer_init_configuration const* config,
                                                         struct ID3D11DeviceContext*                      context);

/**
 * @brief Perform Direct3D 11 graphics rendering.
 *
 * Use this method to perform all graphics rendering - interlacing, sharpening, and GUI display.
 *
 * @param[in] width Width of the render-target.
 * @param[in] height Height of the render-target.
 * @param[in] blendedInterlace Whether to blend the graphics output onto the render-target.
 * @param[in] resultTexture Pointer to destination render-target view.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_d3d11_do_post_process(
    struct leia_interlacer*, int32_t width, int32_t height, leia_bool blendedInterlace, struct ID3D11RenderTargetView* resultTexture);

/**
 * @brief Perform Direct3D 11 graphics rendering for picture content.
 *
 * Use this method to perform all graphics rendering - interlacing, sharpening, and GUI display. This method
 * is similar to DoPostProcess() but allows you to pass in the views to interlace and the destination render-target.
 *
 * @param[in] width Width of the render-target.
 * @param[in] height Height of the render-target.
 * @param[in] pictureShaderResourceView The shader resource view for the views texture.
 * @param[in] renderTargetView Pointer to destination render-target view.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_d3d11_do_post_process_picture(struct leia_interlacer*,
                                                   int32_t                          width,
                                                   int32_t                          height,
                                                   struct ID3D11ShaderResourceView* pictureShaderResourceView,
                                                   struct ID3D11RenderTargetView*   renderTargetView);

/**
 * @brief Set Direct3D 11 view texture.
 *
 * This method allows you to specify the texture for a specific view index and layer.
 *
 * @param[in] texture The view texture.
 * @param[in] viewIndex The view index.
 * @param[in] layer The layer at which to set the view texture.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_d3d11_set_source_views(struct leia_interlacer*, struct ID3D11ShaderResourceView* texture, int32_t viewIndex, int layer);

/*!
 * @}
 */

END_CAPI_DECL

#endif // LEIA_USE_DIRECTX

#endif // CNSDK_LEIA_CORE_INTERLACER_D3D11_H
