#pragma once

///////////////////////////////
// Headers
///////////////////////////////

#include "Device.h"
#include "Image.h"
#include "RenderPass.h"

namespace bl
{

///////////////////////////////
// Classes
///////////////////////////////

struct GeometryPassCreateInfo
{
    GfxDevice* pDevice;
    uint32_t        imageCount;
    VkExtent2D      extent;
};

/// A render pass containing images for geometry d
class BLUEMETAL_API GeometryPass : public RenderPass
{
public:
    GeometryPass();
    GeometryPass(const GeometryPassCreateInfo& createInfo);
    ~GeometryPass();

    /// Creates the geometry pass and generates the images required for rendering.
    [[nodiscard]] bool create(const GeometryPassCreateInfo& createInfo);

    /// Returns the underlying Vulkan render pass object.
    virtual VkRenderPass getHandle();

    /// Resizes images, and framebuffers to match extent.
    virtual void recreate(VkExtent2D extent);

    /// Records this render pass to the framebuffer.
    virtual void record(VkCommandBuffer cmd, VkRect2D renderArea, uint32_t imageIndex);

private:
    bool createImages();
    void destroyImages();
    bool createFramebuffer();
    void destroyFramebuffer();
    bool createPass();
    void destroyPass();

    GfxDevice*                     m_pDevice;
    VkExtent2D                          m_extent;
    VkFormat                            m_albedoSpecularFormat;
    VkFormat                            m_positionFormat;
    VkFormat                            m_normalFormat;
    std::vector<std::unique_ptr<GfxImage>> m_albedoSpecularImages;
    std::vector<std::unique_ptr<GfxImage>> m_positionImages;
    std::vector<std::unique_ptr<GfxImage>> m_normalImages;
    std::vector<VkFramebuffer>          m_geometryFramebuffer;
};

}