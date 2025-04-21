#pragma once

#include "VGE_camera.hpp"

#include <vulkan/vulkan.h>

#include "component.hpp"

namespace VGE
{
    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        game::CameraComponent& camera;
        VkDescriptorSet globalDescriptorSet = VK_NULL_HANDLE;
    };
}