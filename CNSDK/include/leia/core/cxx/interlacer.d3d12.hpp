#ifndef CNSDK_LEIA_CORE_CXX_INTERLACER_D3D12_HPP
#define CNSDK_LEIA_CORE_CXX_INTERLACER_D3D12_HPP

#include "leia/core/cxx/interlacer.hpp"
#include "leia/core/interlacer.d3d12.h"

/*!
 * @addtogroup interlacer_d3d12
 * @{
 */

namespace leia {

/**
 * @brief C++ wrapper for D3D12 leia_interlacer
 */
class InterlacerD3D12 : public Interlacer {
public:
    InterlacerD3D12(Core& core, InterlacerInitConfiguration const& config, ID3D12Device* device, ID3D12CommandQueue* commandQueue)
        : Interlacer(leia_interlacer_d3d12_initialize(core.GetHandle(), config.GetHandle(), device, commandQueue), true)
    {
    }
    InterlacerD3D12(InterlacerD3D12 const&) = delete;
    InterlacerD3D12& operator=(InterlacerD3D12 const&) = delete;
    InterlacerD3D12(InterlacerD3D12&&) = delete;
    InterlacerD3D12& operator=(InterlacerD3D12&&) = delete;

    InterlacerD3D12(leia_interlacer* nonOwningInterlacer) : Interlacer(nonOwningInterlacer, false)
    {
    }

    void DoPostProcess(int32_t width, int32_t height, bool blendedInterlace, ID3D12Resource* renderTarget)
    {
        leia_interlacer_d3d12_do_post_process(_interlacer, width, height, blendedInterlace, renderTarget);
    }
    void DoPostProcessPicture(int32_t width, int32_t height, ID3D12Resource* pictureResource, ID3D12Resource* renderTarget)
    {
        leia_interlacer_d3d12_do_post_process_picture(_interlacer, width, height, pictureResource, renderTarget);
    }

    /**
     * @brief Set Direct3D 12 view texture.
     *
     * This method allows you to specify the texture for a specific view index and layer.
     *
     * @param[in] resource The view texture.
     * @param[in] viewIndex The view index.
     * @param[in] layer The layer at which to set the view texture.
     *
     * @public @memberof InterlacerD3D12
     */
    void SetSourceViews(ID3D12Resource* resource, int viewIndex = 0, int layer = 0)
    {
        leia_interlacer_d3d12_set_source_views(_interlacer, resource, viewIndex, layer);
    }
};

LEIA_NODISCARD
inline InterlacerD3D12* AsD3D12(Interlacer* interlacer)
{
    if (interlacer && interlacer->GetGraphicsAPI() == LEIA_GRAPHICS_API_D3D12)
    {
        return static_cast<InterlacerD3D12*>(interlacer);
    }
    return nullptr;
}

} // namespace leia

/*!
 * @}
 */

#endif // CNSDK_LEIA_CORE_CXX_INTERLACER_D3D12_HPP
