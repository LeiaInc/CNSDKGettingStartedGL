#ifndef CNSDK_LEIA_CORE_INTERLACER_OPENGL_H
#define CNSDK_LEIA_CORE_INTERLACER_OPENGL_H

#include "leia/core/interlacer.h"

#if defined(LEIA_USE_OPENGL)

BEGIN_CAPI_DECL

/*!
 * @defgroup interlacer_opengl OpenGL Interlacer
 * @ingroup interlacer
 *
 * @brief OpenGL interlacing on Leia devices.
 */

/*!
 * @addtogroup interlacer_opengl
 * @{
 */

/**
 * @brief Initialize interlacer for usage with OpenGL.
 *
 * If your application uses OpenGL or OpenGL-ES, use this method to initialize.
 *
 * @param[in] context OpenGL context.
 *
 * @relates leia_interlacer
 */
LEIA_NODISCARD
LEIASDK_API
struct leia_interlacer* leia_interlacer_opengl_initialize(struct leia_core* core, struct leia_interlacer_init_configuration const* config, void* context);

/**
 * @brief Perform OpenGL graphics rendering.
 *
 * Use this method to perform all graphics rendering - interlacing, sharpening, and GUI display.
 *
 * @param[in] width Width of the render-target.
 * @param[in] height Height of the render-target.
 * @param[in] blendedInterlace Whether to blend the graphics output onto the render-target.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_opengl_do_post_process(struct leia_interlacer*, int32_t width, int32_t height, leia_bool blendedInterlace);

/**
 * @brief Perform OpenGL graphics rendering for texture content.
 *
 * Use this method to perform all graphics rendering - interlacing, sharpening, and GUI display. This method
 * is similar to leia_interlacer_opengl_do_post_process() but allows you to pass in the views to interlace.
 *
 * @param[in] width Width of the render-target.
 * @param[in] height Height of the render-target.
 * @param[in] views Name of OpenGL views texture.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_opengl_do_post_process_picture(struct leia_interlacer*, int32_t width, int32_t height, uint32_t views);

/**
 * @brief Perform OpenGL graphics rendering for video content.
 *
 * Use this method to perform all graphics rendering - interlacing, sharpening, and GUI display. This method
 * is similar to leia_interlacer_opengl_do_post_process() but allows you to pass in the views to interlace.
 *
 * @param[in] width Width of the render-target.
 * @param[in] height Height of the render-target.
 * @param[in] views Name of OpenGL views texture.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_opengl_do_post_process_video(struct leia_interlacer*, int32_t width, int32_t height, uint32_t views);

/**
 * @brief Set the OpenGL output texture.
 *
 * To render to a render-target other than the default backbuffer, use this method to specify it's name.
 *
 * @param[in] renderTarget Name of views OpenGL render-target.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_opengl_set_output_render_target(struct leia_interlacer*, uint32_t renderTarget);

/**
 * @brief Set the OpenGL view texture.
 *
 * This method allows you to specify the texture for a specific view index and layer.
 *
 * @param[in] glTextureId The view texture.
 * @param[in] viewIndex The view index.
 * @param[in] layer The layer at which to set the view texture.
 *
 * @public @memberof leia_interlacer
 */
LEIASDK_API
void leia_interlacer_opengl_set_source_views(struct leia_interlacer*, uint32_t glTextureId, int32_t viewIndex, int layer);

/*!
 * @}
 */

END_CAPI_DECL

#endif // LEIA_USE_OPENGL

#endif // CNSDK_LEIA_CORE_INTERLACER_OPENGL_H
