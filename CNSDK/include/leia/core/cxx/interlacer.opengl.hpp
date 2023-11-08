#ifndef CNSDK_LEIA_CORE_CXX_INTERLACER_OPENGL_HPP
#define CNSDK_LEIA_CORE_CXX_INTERLACER_OPENGL_HPP

#include "leia/core/cxx/interlacer.hpp"
#include "leia/core/interlacer.opengl.h"

/*!
 * @addtogroup interlacer_opengl
 * @{
 */

namespace leia {

/**
 * @brief C++ wrapper for OpenGL leia_interlacer
 */
class InterlacerOpenGL : public Interlacer {
public:
    InterlacerOpenGL(Core& core, InterlacerInitConfiguration const& config, void* context)
        : Interlacer(leia_interlacer_opengl_initialize(core.GetHandle(), config.GetHandle(), context), true)
    {
    }
    InterlacerOpenGL(InterlacerOpenGL const&) = delete;
    InterlacerOpenGL& operator=(InterlacerOpenGL const&) = delete;
    InterlacerOpenGL(InterlacerOpenGL&&) = delete;
    InterlacerOpenGL& operator=(InterlacerOpenGL&&) = delete;

    void DoPostProcess(int32_t width, int32_t height, bool blendedInterlace)
    {
        leia_interlacer_opengl_do_post_process(_interlacer, width, height, blendedInterlace);
    }
    void DoPostProcessPicture(int32_t width, int32_t height, uint32_t views)
    {
        leia_interlacer_opengl_do_post_process_picture(_interlacer, width, height, views);
    }
    void DoPostProcessVideo(int32_t width, int32_t height, uint32_t views)
    {
        leia_interlacer_opengl_do_post_process_video(_interlacer, width, height, views);
    }
    void SetOutputRenderTarget(uint32_t renderTarget)
    {
        leia_interlacer_opengl_set_output_render_target(_interlacer, renderTarget);
    }

    /**
     * @brief Set the OpenGL view texture.
     *
     * This method allows you to specify the texture for a specific view index and layer.
     *
     * @param[in] glTextureId The view texture.
     * @param[in] viewIndex The view index.
     * @param[in] layer The layer at which to set the view texture.
     *
     * @public @memberof InterlacerOpenGL
     */
    void SetSourceViews(uint32_t glTextureId, int32_t viewIndex = 0, int layer = 0)
    {
        leia_interlacer_opengl_set_source_views(_interlacer, glTextureId, viewIndex, layer);
    }

    InterlacerOpenGL(leia_interlacer* nonOwningInterlacer) : Interlacer(nonOwningInterlacer, false)
    {
    }
};

LEIA_NODISCARD
inline InterlacerOpenGL* AsOpenGL(Interlacer* interlacer)
{
    if (interlacer && interlacer->GetGraphicsAPI() == LEIA_GRAPHICS_API_OPENGL)
    {
        return static_cast<InterlacerOpenGL*>(interlacer);
    }
    return nullptr;
}

} // namespace leia

/*!
 * @}
 */

#endif // CNSDK_LEIA_CORE_CXX_INTERLACER_OPENGL_HPP
