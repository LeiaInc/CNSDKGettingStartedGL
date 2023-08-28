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
#ifndef CNSDK_LEIA_CORE_INTERLACER_D3D12_HPP
#define CNSDK_LEIA_CORE_INTERLACER_D3D12_HPP

#include "leia/sdk/core.interlacer.hpp"
#include "leia/sdk/core.interlacer.d3d12.h"

/*!
 * @addtogroup interlacer_d3d12
 * @{
 */

namespace leia {
namespace sdk {

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
    void SetInterlaceViewTextureAtlas(ID3D12Resource* texture, int layer = 0)
    {
        leia_interlacer_d3d12_set_interlace_view_texture_atlas(_interlacer, texture, layer);
    }
    void SetViewForTextureArray(int viewIndex, ID3D12Resource* resource, int layer = 0)
    {
        leia_interlacer_d3d12_set_view_for_texture_array(_interlacer, viewIndex, resource, layer);
    }
    void SetViewTextureId(ID3D12Resource* view)
    {
        leia_interlacer_d3d12_set_view_texture_id(_interlacer, view);
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

} // namespace sdk
} // namespace leia

/*!
 * @}
 */

#endif // CNSDK_LEIA_CORE_INTERLACER_D3D12_HPP
