#pragma once

#include "Render/Instance.h"
#include "Window/Window.h"

class BLUEMETAL_API blDevice
{
public:
    explicit blDevice(std::shared_ptr<blInstance> instance, std::shared_ptr<blWindow> temporaryWindow);
    ~blDevice();

    VkInstance getInstance();
    VkDevice getDevice();
    VkPhysicalDevice getPhysicalDevice();
    uint32_t getGraphicsFamilyIndex();
    uint32_t getPresentFamilyIndex();
    VkQueue getGraphicsQueue();
    VkQueue getPresentQueue();
    VkCommandPool getCommandPool();
    VmaAllocator getAllocator();
    bool areQueuesSame();
    std::string getVendorName();
    std::string getDeviceName();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    void immediateSubmit(const std::function<void(VkCommandBuffer)>& recorder);
    void waitForDevice();
    
private:
    std::vector<const char*> getValidationLayers();
    std::vector<const char*> getExtensions();
    void createDevice(std::shared_ptr<blWindow> window);
    void createCommandPool();
    void createAllocator();

    std::shared_ptr<blInstance> _instance;
    uint32_t                    _graphicsFamilyIndex, _presentFamilyIndex;
    VkDevice                    _device;
    VkQueue                     _graphicsQueue, _presentQueue;
    VkCommandPool               _commandPool;
    VmaAllocator                _allocator;
};