#include "VulkanDevice.h"
#include "VulkanPipelineLayoutCache.h"

namespace bl 
{

bool VulkanPipelineLayoutCacheData::operator==(const VulkanPipelineLayoutCacheData& rhs) const noexcept
{
    if (layouts != rhs.layouts) return false;
    for (size_t i = 0; i < ranges.size(); i++)
    {
        const auto& r = ranges[i];
        const auto& rr = rhs.ranges[i];

        if (r.offset != rr.offset
            || r.size != rr.size
            || r.stageFlags != rr.stageFlags) return false;
    }

    return true;
}

std::size_t VulkanPipelineLayoutCacheHasher::operator()(const VulkanPipelineLayoutCacheData& data) const noexcept
{
    std::size_t seed = 0;
    for (const auto& layout : data.layouts)
        seed = hash_combine(seed, layout);

    for (const auto& range : data.ranges)
    {
        seed = hash_combine(seed, range.stageFlags);
        seed = hash_combine(seed, range.offset);
        seed = hash_combine(seed, range.size);
    }

    return seed;
}

VulkanPipelineLayoutCache::VulkanPipelineLayoutCache(VulkanDevice* device)
    : _device(device)
{
}

VulkanPipelineLayoutCache::~VulkanPipelineLayoutCache()
{
    for (auto& pair : _cache)
        vkDestroyPipelineLayout(_device->Get(), pair.second, nullptr);
}

VkPipelineLayout VulkanPipelineLayoutCache::Acquire(const std::span<VkDescriptorSetLayout> layouts, const std::span<VkPushConstantRange> ranges)
{
    VulkanPipelineLayoutCacheData data = {};
    data.layouts.assign(layouts.begin(), layouts.end());
    data.ranges.assign(ranges.begin(), ranges.end());

    // std::sort(data.layouts.begin(), data.layouts.end()); // Turns out that descriptor set layouts are ordered in only some drivers.

    // Sort each value to ensure stability when hashing.
    std::sort(data.ranges.begin(), data.ranges.end(), 
        [](VkPushConstantRange& a, VkPushConstantRange& b){
            return a.offset < b.offset;
        });

    // Check if the layout is in our cache.
    auto it = _cache.find(data);
    if (it != _cache.end())
        return (*it).second;

    VkPipelineLayoutCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.setLayoutCount = (uint32_t)data.layouts.size();
    createInfo.pSetLayouts = data.layouts.data();
    createInfo.pushConstantRangeCount = (uint32_t)data.ranges.size();
    createInfo.pPushConstantRanges = data.ranges.data();

    VkPipelineLayout layout = VK_NULL_HANDLE;
    VK_CHECK(vkCreatePipelineLayout(_device->Get(), &createInfo, nullptr, &layout))

    // Add the new layout to the cache.
    _cache[data] = layout;
    return layout;
}

} // namespace bl