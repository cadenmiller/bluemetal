#pragma once

namespace bl
{

class Describable /** @brief Any vulkan object that can be referenced from a descriptor. */
{
public:
    Describable();
    ~Describable();
};

} // namespace bl