#pragma once

#include "Device.h"

namespace bl
{

class BLUEMETAL_API Shader
{
public:
    Shader(std::shared_ptr<Device> device, VkShaderStageFlagBits stage, const std::vector<uint32_t>& binary);
    ~Shader();


    VkShaderStageFlagBits getStage();
    VkShaderModule getHandle();
    std::vector<uint32_t> getBinary();

private:
    void createModule();

    std::shared_ptr<Device> m_device;
    VkShaderModule          m_module;
    VkShaderStageFlagBits   m_stage;
    std::vector<uint32_t>   m_binary;
};

} // namespace bl