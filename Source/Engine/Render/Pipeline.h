#pragma once

#include "Device.h"
#include "Shader.h"
#include "DescriptorLayoutCache.h"
#include "PipelineLayoutCache.h"
#include "RenderPass.h"

class BLUEMETAL_API blPipeline
{
public:
    explicit blPipeline(std::shared_ptr<blDevice> device, std::shared_ptr<blPipelineLayoutCache> pipelineLayoutCache, std::shared_ptr<blDescriptorLayoutCache> descriptorLayoutCache, const std::vector<std::shared_ptr<blShader>>& shaders, std::shared_ptr<blRenderPass> renderPass, uint32_t subpass = 0);
    ~blPipeline() noexcept;

    VkPipelineLayout getPipelineLayout();
    VkPipeline getPipeline();
    std::vector<VkDescriptorSetLayout> getDescriptorSetLayouts();

private:
    void createDescriptorSetLayouts(const std::vector<std::shared_ptr<blShader>>& shaders);
    void createLayout();
    void createPipeline(const std::vector<std::shared_ptr<blShader>>& shaders);

    std::shared_ptr<blDevice>                   _device;
    std::shared_ptr<blRenderPass>               _renderPass;
    uint32_t                                    _subpass;
    std::vector<VkDescriptorSetLayout>          _setLayouts;
    VkPipelineLayout                            _pipelineLayout;
    VkPipeline                                  _pipeline;
};


/* Getting real with descriptor sets.

    Descriptor Slot: 0
        Reserved for global data of the frame.
        
        This Includes
            * Current Time
            * Camera MVP
            * Resolution
            * Texture Atlas
            * ... (TBD)
    
    Descriptor Slot: 1
        Reserved for material data. Will change multiple times per frame. Each
        material instance's data will be uploaded here.

        Material data is custom to the shader and pipelines will agree to what
        ever the shader says because of reflection. The material system will 
        also take from the pipelines reflection and build a list of values that
        can be changed at runtime using this descriptor index.

        Pipelines are essentially here on the order.

    Descriptor Slot: 2
        Reserved for object data. This will change many times during the runtime
        of a singe frame. 

        This Includes:
            * Object/Mesh Transform
            * ... (TBD)

    Descriptor Slot: 3
        To be determined, might be for instanced data and/or textures?
        Instanced data might need to go on slot 2.

*/