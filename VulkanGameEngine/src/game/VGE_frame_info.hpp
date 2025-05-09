#pragma once

#include <vulkan/vulkan.h>

#include "component.hpp"

namespace VGE
{

    #define MAX_LIGHTS 20

    struct PointLight
    {
        glm::vec4 position{0.0f};
        glm::vec4 color{1.0f}; // w is intensity
    };

    struct GlobalUBO
    {
        alignas(16) glm::mat4 projectionMatrix{1.0f};
        alignas(16) glm::mat4 ViewMatrix{1.0f};
        alignas(16) glm::vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.02f};
        alignas(16) PointLight pointLights[MAX_LIGHTS];
        int numPointLights = 0;
        //alignas(16) glm::vec3 lightDirection = glm::normalize(glm::vec3{4.0f, -15.0f, -3.0f});
    };

    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        game::CameraComponent& camera;
        VkDescriptorSet globalDescriptorSet = VK_NULL_HANDLE;
    };
}