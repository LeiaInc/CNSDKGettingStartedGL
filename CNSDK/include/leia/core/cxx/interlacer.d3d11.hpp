#ifndef CNSDK_LEIA_CORE_CXX_INTERLACER_D3D11_HPP
#define CNSDK_LEIA_CORE_CXX_INTERLACER_D3D11_HPP

#include "leia/core/cxx/interlacer.hpp"
#include "leia/core/interlacer.d3d11.h"

/*!
 * @addtogroup interlacer_d3d11
 * @{
 */

namespace leia {

/**
 * @brief C++ wrapper for D3D11 leia_interlacer
 */
class InterlacerD3D11 : public Interlacer {
public:
    InterlacerD3D11(Core& core, InterlacerInitConfiguration const& config, ID3D11DeviceContext* context)
        : Interlacer(leia_interlacer_d3d11_initialize(core.GetHandle(), config.GetHandle(), context), true)
    {
    }
    InterlacerD3D11(InterlacerD3D11 const&) = delete;
    InterlacerD3D11& operator=(InterlacerD3D11 const&) = delete;
    InterlacerD3D11(InterlacerD3D11&&) = delete;
    InterlacerD3D11& operator=(InterlacerD3D11&&) = delete;

    void DoPostProcess(int width, int height, bool blendedInterlace, ID3D11RenderTargetView* resultTexture)
    {
        leia_interlacer_d3d11_do_post_process(_interlacer, width, height, blendedInterlace, resultTexture);
    }
    void DoPostProcessPicture(int width, int height, ID3D11ShaderResourceView* pictureShaderResourceView, ID3D11RenderTargetView* renderTargetView)
    {
        leia_interlacer_d3d11_do_post_process_picture(_interlacer, width, height, pictureShaderResourceView, renderTargetView);
    }

    /**
     * @brief Set Direct3D 11 view texture.
     *
     * This method allows you to specify the texture for a specific view index and layer.
     *
     * @param[in] texture The view texture.
     * @param[in] viewIndex The view index.
     * @param[in] layer The layer at which to set the view texture.
     *
     * @public @memberof InterlacerD3D11
     */
    void SetSourceViews(ID3D11ShaderResourceView* texture, int viewIndex = 0, int layer = 0)
    {
        leia_interlacer_d3d11_set_source_views(_interlacer, texture, viewIndex, layer);
    }

    InterlacerD3D11(leia_interlacer* nonOwningInterlacer) : Interlacer(nonOwningInterlacer, false)
    {
    }
};

LEIA_NODISCARD
inline InterlacerD3D11* AsD3D11(Interlacer* interlacer)
{
    if (interlacer && interlacer->GetGraphicsAPI() == LEIA_GRAPHICS_API_D3D11)
    {
        return static_cast<InterlacerD3D11*>(interlacer);
    }
    return nullptr;
}

} // namespace leia

/*!
 * @}
 */

#endif // CNSDK_LEIA_CORE_CXX_INTERLACER_D3D11_HPP
