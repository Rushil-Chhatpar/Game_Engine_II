#pragma once

#include "VGE_camera.hpp"

#include <vulkan/vulkan.h>

namespace VGE
{
    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        VgeCamera& camera;
        VkDescriptorSet globalDescriptorSet = VK_NULL_HANDLE;
    };
}