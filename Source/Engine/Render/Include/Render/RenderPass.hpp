#pragma once

#include "Render/RenderDevice.hpp"
#include "Render/Export.h"

using blRenderPassFunction = std::function<void(VkCommandBuffer)>;

class BLOODLUST_RENDER_API blRenderPass
{
public:
    blRenderPass(
        std::shared_ptr<const blRenderDevice> renderDevice,
        const blRenderPassFunction& func);
    virtual ~blRenderPass();

    vk::RenderPass getRenderPass() const noexcept;

    virtual void resize(blExtent2D extent);
    virtual void record(VkCommandBuffer cmd, uint32_t index);
    
protected:
    std::shared_ptr<const blRenderDevice> _renderDevice;
    vk::UniqueRenderPass _pass;
    blRenderPassFunction _func;
    blExtent2D _extent;
};